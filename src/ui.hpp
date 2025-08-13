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
#include "pid.hpp"
#include "input.hpp"

template <typename Driver>
/**
 * @class RingController
 * @brief Manages the entire UI, including menus and pages.
 */
class RingController {
public:
    Driver& OLED;
    RingController(Driver& oled) : 
        OLED(oled),
        anim_pid(g_config.anim_pid_kp, g_config.anim_pid_ki, g_config.anim_pid_kd),
        scroll_pid(g_config.scroll_pid_kp, g_config.scroll_pid_ki, g_config.scroll_pid_kd)
    {}

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
    void update_pid_gains() {
        anim_pid.set_gains(g_config.anim_pid_kp, g_config.anim_pid_ki, g_config.anim_pid_kd);
        scroll_pid.set_gains(g_config.scroll_pid_kp, g_config.scroll_pid_ki, g_config.scroll_pid_kd);
    }

    void handle(Menu* startMenu) {
        if (!startMenu) return;

        std::vector<Menu*> menuStack;
        menuStack.push_back(startMenu);

        while (true) {
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
                        handlePage(page, currentMenu, selectedItem);
                        delete page; // Clean up the page object after it's done
                    }
                } else if (selectedItem.type == MenuItem::ItemType::SWITCH) {
                    selectedItem.switch_action();
                }
            } else { // Cancelled from a menu
                if (menuStack.size() > 1) {
                    Menu* parentMenu = menuStack[menuStack.size() - 2];
                    // Animate back to the parent menu
                    animateTransition(currentMenu, parentMenu, ANIM_BACKWARD);
                    menuStack.pop_back();
                }
            }
        }
    }

private:
    PIDController anim_pid;
    PIDController scroll_pid;

    enum anim_direction {
        ANIM_FORWARD,
        ANIM_BACKWARD
    };

    /**
     * @brief Manages the lifecycle of a page, including animations and interaction.
     * @param page A pointer to the page to handle.
     * @param under_menu The menu that is displayed underneath the page during animations.
     */
    void handlePage(Page* page, Menu* under_menu, MenuItem& item) {
        double current_y, target_y;
        double velocity_y = 0.0;
        int menu_y_offset = calculate_scroll_offset(under_menu);

        // --- Page Entry Animation ---
        current_y = -SCREEN_HEIGHT;
        target_y = 0;
        anim_pid.reset();
        while (abs(target_y - current_y) > 0.1 || abs(velocity_y) > 0.1) {
            velocity_y = anim_pid.update(target_y, current_y);
            current_y += velocity_y;

            OLED.clearBuffer();
            OLED.setDrawColor(1);
            drawMenu(under_menu, 0, menu_y_offset);
            page->draw(round(current_y));
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

        // If the item has a close callback, execute it.
        if (item.on_close_callback) {
            item.on_close_callback();
        }

        // --- Page Exit Animation ---
        current_y = 0;
        target_y = -SCREEN_HEIGHT;
        velocity_y = 0.0;
        anim_pid.reset();
        while (abs(target_y - current_y) > 0.1 || abs(velocity_y) > 0.1) {
            velocity_y = anim_pid.update(target_y, current_y);
            current_y += velocity_y;

            OLED.clearBuffer();
            OLED.setDrawColor(1);
            drawMenu(under_menu, 0, menu_y_offset);
            page->draw(round(current_y));
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
            if (menu->getItem(i).type == MenuItem::ItemType::SWITCH) {
                String state = menu->getItem(i).get_switch_state() ? "[ON]" : "[OFF]";
                int state_width = OLED.getStrWidth(state.c_str());
                OLED.setCursor(SCREEN_WIDTH - state_width - DEFAULT_TEXT_MARGIN, 
                            i * DEFAULT_TEXT_HEIGHT + DEFAULT_TEXT_HEIGHT - DEFAULT_TEXT_MARGIN + y_offset);
                OLED.print(state);
            }
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
        double velocity_to = 0.0;
        double x_offset_from;
        int from_y_offset = calculate_scroll_offset(from);

        anim_pid.reset();

        if (direction == ANIM_FORWARD && to == nullptr) {
            double current_x_from = 0;
            double target_x_from = -SCREEN_WIDTH;
            while (abs(target_x_from - current_x_from) > 0.1 || abs(velocity_to) > 0.1) {
                velocity_to = anim_pid.update(target_x_from, current_x_from);
                current_x_from += velocity_to;

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
        double velocity_y = 0.0, velocity_w = 0.0;

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

        PIDController y_pid(g_config.anim_pid_kp, g_config.anim_pid_ki, g_config.anim_pid_kd);
        PIDController w_pid(g_config.anim_pid_kp, g_config.anim_pid_ki, g_config.anim_pid_kd);

        while (abs(target_x_to - current_x_to) > 0.1 || abs(velocity_to) > 0.1) {
            velocity_to = anim_pid.update(target_x_to, current_x_to);
            current_x_to += velocity_to;

            if (direction == ANIM_FORWARD) {
                x_offset_from = current_x_to - SCREEN_WIDTH;
            } else {
                x_offset_from = current_x_to + SCREEN_WIDTH;
            }

            velocity_y = y_pid.update(select_y_target, select_y_current);
            select_y_current += velocity_y;

            velocity_w = w_pid.update(select_w_target, select_w_current);
            select_w_current += velocity_w;

            OLED.clearBuffer();
            OLED.setDrawColor(1);

            drawMenuItems(from, round(x_offset_from), from_y_offset);
            drawMenuItems(to, round(current_x_to), to_y_offset);

            int box_y = round(select_y_current);
            int box_w = round(select_w_current);

            OLED.drawRBox(INIT_CURSOR_X, box_y, box_w + 2 * DEFAULT_TEXT_MARGIN, DEFAULT_TEXT_HEIGHT, 2);
            
            OLED.setDrawColor(0);

            OLED.setClipWindow(INIT_CURSOR_X, box_y, 
                               INIT_CURSOR_X + box_w + 2 * DEFAULT_TEXT_MARGIN, box_y + DEFAULT_TEXT_HEIGHT);

            drawMenuItems(from, round(x_offset_from), from_y_offset);
            drawMenuItems(to, round(current_x_to), to_y_offset);

            OLED.setMaxClipWindow();

            OLED.sendBuffer();
            delay(ANIMATION_DELAY);
        }
    }

    int showMenu(Menu* menu) {
        unsigned long previousMillis_Input = 0;
        
        double currentY = menu->selected * DEFAULT_TEXT_HEIGHT;
        double velocityY = 0.0;

        String initialLabel = menu->getItem(menu->selected).label;
        double currentWidth = OLED.getStrWidth(initialLabel.c_str());
        double velocityW = 0.0;

        scroll_pid.reset();
        PIDController width_pid(g_config.scroll_pid_kp, g_config.scroll_pid_ki, g_config.scroll_pid_kd);

        auto scrollScreen = 0;
        int highlight_screen_y_on_entry = round(currentY) + scrollScreen;
        if (highlight_screen_y_on_entry > SCREEN_HEIGHT - DEFAULT_TEXT_HEIGHT) {
            scrollScreen -= (highlight_screen_y_on_entry - (SCREEN_HEIGHT - DEFAULT_TEXT_HEIGHT));
        } else if (highlight_screen_y_on_entry < 0) {
            scrollScreen -= highlight_screen_y_on_entry;
        }

        while (true) {
            if (g_config.use_serial_control) {
                char serial_cmd = get_serial_input();
                if (serial_cmd != 0) {
                    switch (serial_cmd) {
                        case 'w': // Up
                            if (menu->selected > 0) menu->selected--;
                            break;
                        case 's': // Down
                            if (menu->selected < menu->size() - 1) menu->selected++;
                            break;
                        case 'e': // Confirm
                            return menu->selected;
                        case 'q': // Cancel
                            return -1;
                    }
                }
            }

            if (is_button_pressed(PIN_CANCEL)) {
                return -1;
            }
            if (is_button_pressed(PIN_CONFIRM)) {
                return menu->selected;
            }

            if (millis() - previousMillis_Input >= INPUT_DELAY) {
                previousMillis_Input = millis();

                if (digitalRead(PIN_IS_SCROLLING) == LOW) {
                    if (digitalRead(PIN_SCROLL_TOWARD) == HIGH) { // Down
                        if (menu->selected < menu->size() - 1) menu->selected++;
                    } else { // Up
                        if (menu->selected > 0) menu->selected--;
                    }
                }
            }

            double scrollTargetY = menu->selected * DEFAULT_TEXT_HEIGHT;
            if (abs(scrollTargetY - currentY) > 0.1 || abs(velocityY) > 0.1) {
                velocityY = scroll_pid.update(scrollTargetY, currentY);
                currentY += velocityY;
            } else {
                currentY = scrollTargetY;
            }

            double targetWidth = OLED.getStrWidth(menu->getItem(menu->selected).label.c_str());
            if (abs(targetWidth - currentWidth) > 0.1 || abs(velocityW) > 0.1) {
                velocityW = width_pid.update(targetWidth, currentWidth);
                currentWidth += velocityW;
            } else {
                currentWidth = targetWidth;
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
                if (menu->getItem(i).type == MenuItem::ItemType::SWITCH) {
                    String state = menu->getItem(i).get_switch_state() ? "[ON]" : "[OFF]";
                    int state_width = OLED.getStrWidth(state.c_str());
                    OLED.setCursor(SCREEN_WIDTH - state_width - DEFAULT_TEXT_MARGIN, 
                                i * DEFAULT_TEXT_HEIGHT + DEFAULT_TEXT_HEIGHT - DEFAULT_TEXT_MARGIN + scrollScreen);
                    OLED.print(state);
                }
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
            delay(ANIMATION_DELAY);
        }
    }
};