#include <iostream>
#include <format>
#include <latch>
#include <vector>
#include <future>
#include <random>
#include <atomic>
#include <algorithm>

int main(void)
{
    std::cout << std::format(
        "Welcome to the dungeon, there is boss lying in the deep cave of this dungeon, so be carefull traveler.\n"
        "Fortunately you can bring your heroes as much as you need, also you can specify their damage and crit chance of heroes damage. Cool, huh?\n"
        "But hey, you should be fast, that boss can devour you in just a blink of an eye, "
        "good thing is that, for now he's sleeping, THANK GOD! But as soon as you attack him, it will come at you.\n"
        "As Eminem said: if you had one shot or one opportunity\n"
        "                To seize everything you ever wanted in one moment.\n"
        "                Would you capture it? Or just let it slip?\n"
        "Remeber traveler, if you even defeat the boss, because of the distance of hollow structure of the hall, "
        "you may hear the boss scream even after the death, don't let it to hunt you!! Be strong and faithful.\n"
        "So go on traveler, attack him as powerfull as you can, see if you can defeat the darkness or not.\n");
    
    std::cout << "\033[31mPRESS ENTER TO ENTER THE DUNGEON\033[0m\n";
    std::getchar();


    int hero_count{}, hero_dmg{}, crit_chance{}, difficulty{};
    std::atomic<int> total_dmg{};

    // clean the terminal
    std::cout << "Number of heroes: ";
    std::cin >> hero_count;
    std::cout << "Your heroes DMG (each hero): ";
    std::cin >> hero_dmg;
    std::cout << "Your heroes Crit Chance (each hero, [0 - 100]): ";
    std::cin >> crit_chance;
    std::cout << "Difficulity (0|1|2): ";
    std::cin >> difficulty;

    const double difficulty_factor{
        [difficulty] -> const double
        {
            switch (difficulty)
            {
            case 0:  return 1.1;            
            case 1:  return 1.6;
            case 2:  return 2.0;
            default: return 1.0;
            }
        }()
    };
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.9, 1.2);  // 10% variability
    
    // Calculate boss HP with randomness
    std::atomic<int> boss_hp{ static_cast<int>((hero_count * hero_dmg) * difficulty_factor * dis(gen)) };

    std::cout << std::format("Boss hp: {}\n"
                             "\033[33mPRESS ENTER TO ENTER THE CAVE\033[0m\n",  boss_hp.load());
    std::getchar(); std::getchar();
  
    const std::size_t thread_count{ static_cast<std::size_t>(hero_count) };
    std::latch attack(thread_count);
    std::vector<std::future<void>> attack_threads;
    for (auto i{ 0ul }; i < thread_count; ++i)
        attack_threads.push_back(std::async(std::launch::async, 
            [&, i]
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                
                std::uniform_int_distribution<> crit_distribution(1, 100);
                bool is_crit{ (crit_distribution(gen) <= crit_chance) };
                int dealt_dmg{ (is_crit ? hero_dmg * 2 : hero_dmg) };
                boss_hp -= dealt_dmg;
                total_dmg += dealt_dmg;
                std::cout << std::format("\033[32mHero {} did {}\n", i, (is_crit ? "crit" : "attack"));
                
                attack.count_down();

                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::cout << std::format("\033[31m{} Ouch!!\n", i);
            }));

    attack.wait();

    std::cout << std::format("\033[33mAll of heroes attacked the boss, total damage dealt: {}, boss hp: {}\033[0m\n", 
                             total_dmg.load(), boss_hp.load());

    std::for_each(attack_threads.begin(), attack_threads.end(), [](auto& thread) { thread.wait(); });
    
    std::cout << std::format("\033[0m{}\n", (boss_hp <= 0 ? "HORAAAAAY!!!!!" : "You get eaten, bye bye darling"));

    return 0;
}