#pragma once

#include "ball.h"
#include "buttons.h"
#include "game.h"

LPCWSTR manual =
    L"Stop The Ball\n"
    L"欢迎来到，Stop The Ball ！！！\n"
    L"这是一个考验你的反应的小游戏，相信你会在这里找到乐趣的。\n"
    L"接下来我简单介绍一下：\n"
    L"游戏玩法：顾名思义，Stop The Ball，\n"
    L"阻止这些球，防止上方的进度条增长到100%，不然的话游戏结束。\n"
    L"用鼠标点击球即可阻止绿色球，但请注意还有其他球：\n"
    L"黄色球：需要你快速点击，不然它会脱离你鼠标点击的范围。\n"
    L"蓝色球：有波浪般的移动轨迹\n"
    L"红色球：速度较快，并需点击两下才行\n"
    L"当然，游戏难度并不小，所以我们还提供了强有力的道具：\n"
    L"回复（G）：将进度条变为0%\n"
    L"时光倒流（B）：球的速度开始变慢，并且不会出现新的球，直到画面的球全部消失"
    L"\n"
    L"灭迹（R）：瞬间毁灭掉屏幕出现的所有球。\n"
    L"游戏介绍完了，玩得愉快！！！\n";

inline void show_help(HWND hwnd) {
  MessageBox(hwnd, manual, L"游戏说明", MB_OK);
}

FORCEINLINE void ui_welcome(ik::context& ctx) {
  game.buttons["title"] = std::make_shared<ik::text>();
  game.buttons["title"]->text = L"Stop The Ball";
  game.buttons["title"]->font = ctx.title_font;
  game.buttons["title"]->real_rect(
      ik::type::rect(0, ctx.height / 5, ctx.width, 70));

  game.buttons["start"] = std::make_shared<ik::button>();
  game.buttons["start"]->text = L"开始游戏";
  game.buttons["start"]->real_rect(
      ik::type::rect(0, ctx.height / 3, ctx.width, 50));

  game.buttons["detail"] = std::make_shared<ik::button>();
  game.buttons["detail"]->text = L"游戏说明";
  game.buttons["detail"]->real_rect(
      ik::type::rect(0, ctx.height * 4 / 9, ctx.width, 50));
  game.buttons["detail"]->fns_on_btn_up.push_back(
      {"manual", [&](const ik::type::point& pt) { show_help(ctx.hwnd); }});

  game.buttons["exit"] = std::make_shared<ik::button>();
  game.buttons["exit"]->text = L"退出";
  game.buttons["exit"]->real_rect(
      ik::type::rect(0, ctx.height * 5 / 9, ctx.width, 50));

  game.add_timed_task(ik::type::duration(500),
                      [&](ik::type::time_point now, ik::game& g) {
                        game.buttons["title"]->show();
                      });
  game.add_timed_task(ik::type::duration(1000), [&](ik::type::time_point now,
                                                    ik::game& g) {
    game.buttons["start"]->show();
    game.buttons["start"]->fns_on_btn_up.push_back(
        {"start", [&](const ik::type::point& pt) {
           for (auto& o : game.buttons) o.second->hide();
           game.add_timed_task(ik::type::duration(500), [&](TASK_PARAM) {
             game.message(L"游戏开始", ik::type::duration(1000), ctx);
             game.finish_fn = [&](int score, ik::type::duration d) {
               game.message(L"游戏结束\n得分：" + std::to_wstring(score) +
                                L"\n用时：" +
                                std::to_wstring(d.count() / 1000) + L"秒",
                            ik::type::duration(3000), ctx);
               game.add_timed_task(ik::type::duration(3000),
                                   [&ctx](TASK_PARAM) { ui_welcome(ctx); });
             };
             game.add_timed_task(ik::type::duration(1000),
                                 [&](TASK_PARAM) { game.start(ctx); });
           });
         }});

    game.add_timed_task(ik::type::duration(500),
                        [&](ik::type::time_point now, ik::game& g) {
                          game.buttons["detail"]->show();
                        });

    game.add_timed_task(
        ik::type::duration(1000), [&](ik::type::time_point now, ik::game& g) {
          game.buttons["exit"]->fns_on_btn_up.push_back(
              {"exit", [](const ik::type::point& pt) {
                 for (auto& o : game.buttons) o.second->hide();
                 game.add_timed_task(ik::type::duration(500),
                                     [](TASK_PARAM) { PostQuitMessage(0); });
               }});
          game.buttons["exit"]->show();
        });
  });
}
