#pragma once

#include "ball.h"
#include "buttons.h"
#include "game.h"

LPCWSTR manual =
    L"Stop The Ball\n"
    L"��ӭ������Stop The Ball ������\n"
    L"����һ��������ķ�Ӧ��С��Ϸ����������������ҵ���Ȥ�ġ�\n"
    L"�������Ҽ򵥽���һ�£�\n"
    L"��Ϸ�淨������˼�壬Stop The Ball��\n"
    L"��ֹ��Щ�򣬷�ֹ�Ϸ��Ľ�����������100%����Ȼ�Ļ���Ϸ������\n"
    L"��������򼴿���ֹ��ɫ�򣬵���ע�⻹��������\n"
    L"��ɫ����Ҫ����ٵ������Ȼ����������������ķ�Χ��\n"
    L"��ɫ���в��˰���ƶ��켣\n"
    L"��ɫ���ٶȽϿ죬���������²���\n"
    L"��Ȼ����Ϸ�ѶȲ���С���������ǻ��ṩ��ǿ�����ĵ��ߣ�\n"
    L"�ظ���G��������������Ϊ0%\n"
    L"ʱ�⵹����B��������ٶȿ�ʼ���������Ҳ�������µ���ֱ���������ȫ����ʧ"
    L"\n"
    L"�𼣣�R����˲��������Ļ���ֵ�������\n"
    L"��Ϸ�������ˣ������죡����\n";

inline void show_help(HWND hwnd) {
  MessageBox(hwnd, manual, L"��Ϸ˵��", MB_OK);
}

FORCEINLINE void ui_welcome(ik::context& ctx) {
  game.buttons["title"] = std::make_shared<ik::text>();
  game.buttons["title"]->text = L"Stop The Ball";
  game.buttons["title"]->font = ctx.title_font;
  game.buttons["title"]->real_rect(
      ik::type::rect(0, ctx.height / 5, ctx.width, 70));

  game.buttons["start"] = std::make_shared<ik::button>();
  game.buttons["start"]->text = L"��ʼ��Ϸ";
  game.buttons["start"]->real_rect(
      ik::type::rect(0, ctx.height / 3, ctx.width, 50));

  game.buttons["detail"] = std::make_shared<ik::button>();
  game.buttons["detail"]->text = L"��Ϸ˵��";
  game.buttons["detail"]->real_rect(
      ik::type::rect(0, ctx.height * 4 / 9, ctx.width, 50));
  game.buttons["detail"]->fns_on_btn_up.push_back(
      {"manual", [&](const ik::type::point& pt) { show_help(ctx.hwnd); }});

  game.buttons["exit"] = std::make_shared<ik::button>();
  game.buttons["exit"]->text = L"�˳�";
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
             game.message(L"��Ϸ��ʼ", ik::type::duration(1000), ctx);
             game.finish_fn = [&](int score, ik::type::duration d) {
               game.message(L"��Ϸ����\n�÷֣�" + std::to_wstring(score) +
                                L"\n��ʱ��" +
                                std::to_wstring(d.count() / 1000) + L"��",
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
