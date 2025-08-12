/**
 * @file ui.hpp
 * @brief Defines the main UI controller, RingController.
 */
#pragma once

#include <Arduino.h>
#include <functional>
#include <vector>
#include <U8g2lib.h>
#include "element.hpp"
#include "config.hpp"

#define ENABLE_INTELLISENSE_HACK 1

#if ENABLE_INTELLISENSE_HACK
    using Driver = U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C;
#else
    template <typename Driver>
#endif

/**
 * @class RingController
 * @brief Manages the entire UI, including menus and pages.
 */
class RingController {
public:
    Driver& OLED;
    RingController(Driver& oled) : OLED(oled) {}

    /**
     * @brief Initializes the OLED display.
     */
    void setup() {
        OLED.begin();
        OLED.enableUTF8Print();
        OLED.setFont(DEFAULT_TEXT_FONT);
        OLED.setFontMode(1); // Transparent font mode
    }
    
    /**
     * @brief The main entry point and loop for the UI.
     * @param startMenu The root menu to display.
     */
    void handle(Menu* startMenu) {
        if (!startMenu) return;

        std::vector<Menu*> menuStack;
        menuStack.push_back(startMenu);

        while (!menuStack.empty()) {
            Menu* currentMenu = menuStack.back();
            int selectedIndex = showMenu(currentMenu);

            if (selectedIndex >= 0) { // An item was selected
                MenuItem& selectedItem = currentMenu->getItem(selectedIndex);
                if (selectedItem.type == MenuItem::ItemType::DIRECTORY && selectedItem.subMenu) {
                    // Animate to the submenu
                    selectedItem.subMenu->selected = 0;
                    animateTransition(currentMenu, selectedItem.subMenu, ANIM_FORWARD);
                    menuStack.push_back(selectedItem.subMenu);
                } else if (selectedItem.type == MenuItem::ItemType::OPTION && selectedItem.action) {
                    // Create the page and handle its lifecycle
                    Page* page = selectedItem.action();
                    if (page) {
                        handlePage(page, currentMenu);
                        delete page; // Clean up the page object after it's done
                    }
                }
            } else { // Cancelled from a menu
                Menu* parentMenu = nullptr;
                if (menuStack.size() > 1) {
                    parentMenu = menuStack[menuStack.size() - 2];
                }
                // Animate back to the parent menu
                animateTransition(currentMenu, parentMenu, ANIM_BACKWARD);
                menuStack.pop_back();
            }
        }
        OLED.clearBuffer();
        OLED.setCursor(0, DEFAULT_TEXT_HEIGHT);
        OLED.print("Bye!");
        OLED.sendBuffer();
    }

private:
    enum anim_direction {
        ANIM_FORWARD,
        ANIM_BACKWARD
    };

    /**
     * @brief Manages the lifecycle of a page, including animations and interaction.
     * @param page A pointer to the page to handle.
     * @param under_menu The menu that is displayed underneath the page during animations.
     */
    void handlePage(Page* page, Menu* under_menu) {
        double current_y, target_y;
        double velocity_y = 0.0, integral_y = 0.0, last_error_y = 0.0;
        int menu_y_offset = calculate_scroll_offset(under_menu);

        // --- Page Entry Animation ---
        current_y = -SCREEN_HEIGHT;
        target_y = 0;
        while (abs(target_y - current_y) > 0.1 || abs(velocity_y) > 0.1) {
            double error_y = target_y - current_y;
            integral_y += error_y;
            if (integral_y > 20) integral_y = 20; if (integral_y < -20) integral_y = -20;
            double derivative_y = error_y - last_error_y;
            velocity_y = g_config.anim_pid_kp * error_y + g_config.anim_pid_ki * integral_y + g_config.anim_pid_kd * derivative_y;
            current_y += velocity_y;
            last_error_y = error_y;

            OLED.clearBuffer();
            OLED.setDrawColor(1);
            drawMenu(under_menu, 0, menu_y_offset); // Draw menu underneath
            page->draw(round(current_y)); // Draw page content sliding in
            OLED.sendBuffer();
            delay(ANIMATION_DELAY);
        }

        // --- Page Main Loop ---
        while (true) {
            if (page->handleInput()) {
                break; // Page is finished, exit loop
            }

            OLED.clearBuffer();
            OLED.setDrawColor(1);
            page->draw(0); // Draw page content normally
            OLED.sendBuffer();
            delay(ANIMATION_DELAY);
        }

        // --- Page Exit Animation ---
        current_y = 0;
        target_y = -SCREEN_HEIGHT;
        velocity_y = 0.0; integral_y = 0.0; last_error_y = 0.0;
        while (abs(target_y - current_y) > 0.1 || abs(velocity_y) > 0.1) {
            double error_y = target_y - current_y;
            integral_y += error_y;
            if (integral_y > 20) integral_y = 20; if (integral_y < -20) integral_y = -20;
            double derivative_y = error_y - last_error_y;
            velocity_y = g_config.anim_pid_kp * error_y + g_config.anim_pid_ki * integral_y + g_config.anim_pid_kd * derivative_y;
            current_y += velocity_y;
            last_error_y = error_y;

            OLED.clearBuffer();
            OLED.setDrawColor(1);
            drawMenu(under_menu, 0, menu_y_offset); // Draw menu underneath
            page->draw(round(current_y)); // Draw page content sliding out
            OLED.sendBuffer();
            delay(ANIMATION_DELAY);
        }
    }

    void drawMenuItems(Menu* menu, int x_offset, int y_offset, int skip_index = -1) {
        if (!menu) return;
        for (int i = 0; i < menu->size(); i++) {
            if (i == skip_index) continue;
            OLED.setCursor(x_offset + INIT_CURSOR_X + DEFAULT_TEXT_MARGIN, 
                        i * DEFAULT_TEXT_HEIGHT + DEFAULT_TEXT_HEIGHT - DEFAULT_TEXT_MARGIN + y_offset);
            OLED.print(menu->getItem(i).label);
        }
    }

    void drawMenu(Menu* menu, int x_offset, int y_offset) {
        if (!menu || menu->size() == 0) return;

        double currentY = menu->selected * DEFAULT_TEXT_HEIGHT;
        String selectedLabel = menu->getItem(menu->selected).label;
        double currentWidth = OLED.getStrWidth(selectedLabel.c_str());

        for (int i = 0; i < menu->size(); i++) {
            OLED.setCursor(x_offset + INIT_CURSOR_X + DEFAULT_TEXT_MARGIN, 
                        i * DEFAULT_TEXT_HEIGHT + DEFAULT_TEXT_HEIGHT - DEFAULT_TEXT_MARGIN + y_offset);
            OLED.print(menu->getItem(i).label);
        }
        
        int selected_box_y = round(currentY) + y_offset;

        OLED.drawRBox(x_offset + INIT_CURSOR_X, selected_box_y, 
                    round(currentWidth) + 2 * DEFAULT_TEXT_MARGIN, DEFAULT_TEXT_HEIGHT, 2);
        
        OLED.setDrawColor(0);
        OLED.setClipWindow(x_offset + INIT_CURSOR_X, selected_box_y, 
                        x_offset + INIT_CURSOR_X + round(currentWidth) + 2 * DEFAULT_TEXT_MARGIN, selected_box_y + DEFAULT_TEXT_HEIGHT);

        for (int i = 0; i < menu->size(); i++) {
            OLED.setCursor(x_offset + INIT_CURSOR_X + DEFAULT_TEXT_MARGIN, 
                        i * DEFAULT_TEXT_HEIGHT + DEFAULT_TEXT_HEIGHT - DEFAULT_TEXT_MARGIN + y_offset);
            OLED.print(menu->getItem(i).label);
        }

        OLED.setMaxClipWindow();
    }

    int calculate_scroll_offset(Menu* menu) {
        if (!menu) return 0;
        int scrollScreen = 0;
        int currentY = menu->selected * DEFAULT_TEXT_HEIGHT;
        int highlight_screen_y = currentY + scrollScreen;
        if (highlight_screen_y > SCREEN_HEIGHT - DEFAULT_TEXT_HEIGHT) {
            scrollScreen -= (highlight_screen_y - (SCREEN_HEIGHT - DEFAULT_TEXT_HEIGHT));
        } else if (highlight_screen_y < 0) {
            scrollScreen -= highlight_screen_y;
        }
        return scrollScreen;
    }

    void animateTransition(Menu* from, Menu* to, anim_direction direction) {
        double current_x_to = (direction == ANIM_FORWARD) ? SCREEN_WIDTH : -SCREEN_WIDTH;
        double target_x_to = 0;
        double velocity_to = 0.0, integral_to = 0.0, last_error_to = 0.0;
        double x_offset_from;
        int from_y_offset = calculate_scroll_offset(from);

        if (direction == ANIM_FORWARD && to == nullptr) {
            double current_x_from = 0;
            double target_x_from = -SCREEN_WIDTH;
            while (abs(target_x_from - current_x_from) > 0.1 || abs(velocity_to) > 0.1) {
                double error_to = target_x_from - current_x_from;
                integral_to += error_to;
                if (integral_to > 100) integral_to = 100; if (integral_to < -100) integral_to = -100;
                double derivative_to = error_to - last_error_to;
                velocity_to = g_config.anim_pid_kp * error_to + g_config.anim_pid_ki * integral_to + g_config.anim_pid_kd * derivative_to;
                current_x_from += velocity_to;
                last_error_to = error_to;

                OLED.clearBuffer();
                OLED.setDrawColor(1);
                drawMenu(from, round(current_x_from), from_y_offset);
                OLED.sendBuffer();
                delay(ANIMATION_DELAY);
            }
            return;
        }
        
        int to_y_offset = calculate_scroll_offset(to);

        double select_y_current, select_y_target;
        double select_w_current, select_w_target;
        double velocity_y = 0.0, integral_y = 0.0, last_error_y = 0.0;
        double velocity_w = 0.0, integral_w = 0.0, last_error_w = 0.0;

        if (from) {
            select_y_current = from->selected * DEFAULT_TEXT_HEIGHT + from_y_offset;
            if (from->size() > 0) {
                select_w_current = OLED.getStrWidth(from->getItem(from->selected).label.c_str());
            } else {
                select_w_current = 0;
            }
        } else {
            select_y_current = SCREEN_HEIGHT / 2;
            select_w_current = 0;
        }

        if (to) {
            select_y_target = to->selected * DEFAULT_TEXT_HEIGHT + to_y_offset;
            if (to->size() > 0) {
                select_w_target = OLED.getStrWidth(to->getItem(to->selected).label.c_str());
            } else {
                select_w_target = 0;
            }
        } else {
            select_y_target = select_y_current;
            select_w_target = SCREEN_WIDTH;
        }

        while (abs(target_x_to - current_x_to) > 0.1 || abs(velocity_to) > 0.1) {
            double error_to = target_x_to - current_x_to;
            integral_to += error_to;
            if (integral_to > 100) integral_to = 100; if (integral_to < -100) integral_to = -100;
            double derivative_to = error_to - last_error_to;
            velocity_to = g_config.anim_pid_kp * error_to + g_config.anim_pid_ki * integral_to + g_config.anim_pid_kd * derivative_to;
            current_x_to += velocity_to;
            last_error_to = error_to;

            if (direction == ANIM_FORWARD) {
                x_offset_from = current_x_to - SCREEN_WIDTH;
            } else {
                x_offset_from = current_x_to + SCREEN_WIDTH;
            }

            double error_y = select_y_target - select_y_current;
            integral_y += error_y;
            if (integral_y > 20) integral_y = 20; if (integral_y < -20) integral_y = -20;
            double derivative_y = error_y - last_error_y;
            velocity_y = g_config.anim_pid_kp * error_y + g_config.anim_pid_ki * integral_y + g_config.anim_pid_kd * derivative_y;
            select_y_current += velocity_y;
            last_error_y = error_y;

            double error_w = select_w_target - select_w_current;
            integral_w += error_w;
            if (integral_w > 20) integral_w = 20; if (integral_w < -20) integral_w = -20;
            double derivative_w = error_w - last_error_w;
            velocity_w = g_config.anim_pid_kp * error_w + g_config.anim_pid_ki * integral_w + g_config.anim_pid_kd * derivative_w;
            select_w_current += velocity_w;
            last_error_w = error_w;

            OLED.clearBuffer();
            OLED.setDrawColor(1);

            drawMenuItems(from, round(x_offset_from), from_y_offset, from ? from->selected : -1);
            drawMenuItems(to, round(current_x_to), to_y_offset, to ? to->selected : -1);

            int box_y = round(select_y_current);
            int box_w = round(select_w_current);

            OLED.drawRBox(INIT_CURSOR_X, box_y, box_w + 2 * DEFAULT_TEXT_MARGIN, DEFAULT_TEXT_HEIGHT, 2);
            
            OLED.setDrawColor(0);

            if (from && from->size() > 0) {
                OLED.setCursor(round(x_offset_from) + INIT_CURSOR_X + DEFAULT_TEXT_MARGIN, box_y + DEFAULT_TEXT_HEIGHT - DEFAULT_TEXT_MARGIN);
                OLED.print(from->getItem(from->selected).label);
            }
            if (to && to->size() > 0) {
                OLED.setCursor(round(current_x_to) + INIT_CURSOR_X + DEFAULT_TEXT_MARGIN, box_y + DEFAULT_TEXT_HEIGHT - DEFAULT_TEXT_MARGIN);
                OLED.print(to->getItem(to->selected).label);
            }

            OLED.sendBuffer();
            delay(ANIMATION_DELAY);
        }
    }

    int showMenu(Menu* menu) {
        unsigned long previousMillis_Anime = 0;
        unsigned long previousMillis_Input = 0;
        
        double currentY = menu->selected * DEFAULT_TEXT_HEIGHT;
        double scrollTarget = currentY;
        double velocity = 0.0;
        double integral = 0.0;
        double last_error = 0.0;

        String initialLabel = menu->getItem(menu->selected).label;
        double currentWidth = OLED.getStrWidth(initialLabel.c_str());
        double targetWidth = currentWidth;
        double velocityW = 0.0;
        double integralW = 0.0;
        double last_errorW = 0.0;

        auto scrollScreen = 0;
        int highlight_screen_y_on_entry = round(currentY) + scrollScreen;
        if (highlight_screen_y_on_entry > SCREEN_HEIGHT - DEFAULT_TEXT_HEIGHT) {
            scrollScreen -= (highlight_screen_y_on_entry - (SCREEN_HEIGHT - DEFAULT_TEXT_HEIGHT));
        } else if (highlight_screen_y_on_entry < 0) {
            scrollScreen -= highlight_screen_y_on_entry;
        }

        while (true) {
            unsigned long currentMillis = millis();

            if (digitalRead(PIN_CANCEL) == HIGH) {
                delay(50);
                if (digitalRead(PIN_CANCEL) == HIGH) {
                    while(digitalRead(PIN_CANCEL) == HIGH);
                    return -1;
                }
            }
            if (digitalRead(PIN_CONFIRM) == HIGH) {
                delay(50);
                if (digitalRead(PIN_CONFIRM) == HIGH) {
                    while(digitalRead(PIN_CONFIRM) == HIGH);
                    return menu->selected;
                }
            }

            if (currentMillis - previousMillis_Input >= INPUT_DELAY) {
                previousMillis_Input = currentMillis;

                if (digitalRead(PIN_IS_SCROLLING) == LOW) {
                    if (digitalRead(PIN_SCROLL_TOWARD) == HIGH) { // Down
                        if (menu->selected < menu->size() - 1) menu->selected++;
                    } else { // Up
                        if (menu->selected > 0) menu->selected--;
                    }
                }
            }

            if (currentMillis - previousMillis_Anime >= ANIMATION_DELAY) {
                previousMillis_Anime = currentMillis;

                scrollTarget = menu->selected * DEFAULT_TEXT_HEIGHT;

                if (abs(scrollTarget - currentY) > 0.1 || abs(velocity) > 0.1) {
                    double error = scrollTarget - currentY;
                    integral += error;
                    if (integral > 20) integral = 20;
                    if (integral < -20) integral = -20;
                    double derivative = error - last_error;
                    velocity = g_config.scroll_pid_kp * error + g_config.scroll_pid_ki * integral + g_config.scroll_pid_kd * derivative;
                    currentY += velocity;
                    last_error = error;
                } else {
                    currentY = scrollTarget;
                    velocity = 0.0;
                    integral = 0.0;
                    last_error = 0.0;
                }

                targetWidth = OLED.getStrWidth(menu->getItem(menu->selected).label.c_str());
                if (abs(targetWidth - currentWidth) > 0.1 || abs(velocityW) > 0.1) {
                    double errorW = targetWidth - currentWidth;
                    integralW += errorW;
                    if (integralW > 20) integralW = 20;
                    if (integralW < -20) integralW = -20;
                    double derivativeW = errorW - last_errorW;
                    velocityW = g_config.scroll_pid_kp * errorW + g_config.scroll_pid_ki * integralW + g_config.scroll_pid_kd * derivativeW;
                    currentWidth += velocityW;
                    last_errorW = errorW;
                } else {
                    currentWidth = targetWidth;
                    velocityW = 0.0;
                    integralW = 0.0;
                    last_errorW = 0.0;
                }

                int highlight_screen_y = round(currentY) + scrollScreen;
                if (highlight_screen_y > SCREEN_HEIGHT - DEFAULT_TEXT_HEIGHT) {
                    scrollScreen -= (highlight_screen_y - (SCREEN_HEIGHT - DEFAULT_TEXT_HEIGHT));
                } else if (highlight_screen_y < 0) {
                    scrollScreen -= highlight_screen_y;
                }

                OLED.clearBuffer();
                OLED.setDrawColor(1);

                for (int i = 0; i < menu->size(); i++) {
                    OLED.setCursor(INIT_CURSOR_X + DEFAULT_TEXT_MARGIN, 
                                i * DEFAULT_TEXT_HEIGHT + DEFAULT_TEXT_HEIGHT - DEFAULT_TEXT_MARGIN + scrollScreen);
                    OLED.print(menu->getItem(i).label);
                }
                
                int selected_box_y = round(currentY) + scrollScreen;

                OLED.drawRBox(INIT_CURSOR_X, selected_box_y, 
                            round(currentWidth) + 2 * DEFAULT_TEXT_MARGIN, DEFAULT_TEXT_HEIGHT, 2);
                
                OLED.setDrawColor(0);
                OLED.setClipWindow(INIT_CURSOR_X, selected_box_y, 
                                INIT_CURSOR_X + round(currentWidth) + 2 * DEFAULT_TEXT_MARGIN, selected_box_y + DEFAULT_TEXT_HEIGHT);

                for (int i = 0; i < menu->size(); i++) {
                    OLED.setCursor(INIT_CURSOR_X + DEFAULT_TEXT_MARGIN, 
                                i * DEFAULT_TEXT_HEIGHT + DEFAULT_TEXT_HEIGHT - DEFAULT_TEXT_MARGIN + scrollScreen);
                    OLED.print(menu->getItem(i).label);
                }

                OLED.setMaxClipWindow();
                OLED.sendBuffer();
            }
        }
    }
};