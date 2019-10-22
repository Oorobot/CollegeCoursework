#pragma once

#include "buttons.h"
#include "game.h"

FORCEINLINE void ui_welcome(ik::context& ctx) {
  game.add_task(ik::type::duration(500),
                [&](ik::type::time_point now, ik::game& g) {
                  game.buttons["title"] = std::make_shared<ik::text>();
                  game.buttons["title"]->text = L"Stop The Ball";
                  game.buttons["title"]->font = ctx.title_font;
                  game.buttons["title"]->real_rect(
                      ik::type::rect(0, ctx.height / 5, ctx.width, 50));
                  game.buttons["title"]->show();
                });
  game.add_task(ik::type::duration(1000), [&](ik::type::time_point now,
                                              ik::game& g) {
    game.buttons["start"] = std::make_shared<ik::button>();
    game.buttons["start"]->text = L"开始游戏";
    game.buttons["start"]->real_rect(
        ik::type::rect(0, ctx.height / 3, ctx.width, 50));
    game.buttons["start"]->show();
    game.buttons["start"]->fns_on_btn_up.push_back(
        {"start", [](const ik::type::point& pt) {
           for (auto& o : game.buttons) o.second->hide();
         }});

    game.add_task(ik::type::duration(500),
                  [&](ik::type::time_point now, ik::game& g) {
                    game.buttons["detail"] = std::make_shared<ik::button>();
                    game.buttons["detail"]->text = L"游戏说明";
                    game.buttons["detail"]->real_rect(
                        ik::type::rect(0, ctx.height * 4 / 9, ctx.width, 50));
                    game.buttons["detail"]->show();
                  });

    game.add_task(
        ik::type::duration(1000), [&](ik::type::time_point now, ik::game& g) {
          game.buttons["exit"] = std::make_shared<ik::button>();
          game.buttons["exit"]->text = L"退出";
          game.buttons["exit"]->real_rect(
              ik::type::rect(0, ctx.height * 5 / 9, ctx.width, 50));
          game.buttons["exit"]->fns_on_btn_up.push_back(
              {"exit", [](const ik::type::point& pt) {
                 for (auto& o : game.buttons) o.second->hide();
                 game.add_task(ik::type::duration(500),
                               [](TASK_PARAM) { PostQuitMessage(0); });
               }});
          game.buttons["exit"]->show();
        });
  });
}
