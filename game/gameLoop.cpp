#include <ncurses.h>
#include <vector>
#include "../player/character.h"
#include "../enemies/enemy.h"
#include "../levels/levelManagement/level.h"
#include "../levels/levelManagement/levelManager.h"
#include "../screenManager.h"
#include "interface.h"
#include "../player/inputState.h"
#include "../inventory/InventoryIncludes.h"
#include "gameLoop.h"
#include "../player/controls.h"
#include "../timer.h"
#include "../enemies/MonsterGenerator.h"



void gameLoop(int mode) {
    size_t currentLevelIndex = 0;
     if (mode == 1){
             bool gameRunning = true;
            MonsterGenerator::loadMonsterSprites("enemies/monster_sprites.txt");
        while (gameRunning) {
            while (currentLevelIndex < LevelManager::countLevels()) {
                
                Level& level = LevelManager::getLevel(currentLevelIndex);
                MonsterGenerator::generateMonsters(level,currentLevelIndex+1);
                //LevelManager::displayLevel(level);
                //saveScreen(); // Сохраняем весь экран
                saveScreen(level.map);
                Character hero("game.log");  // Создаем объект персонажа с логгером
                std::vector<std::string> heroSprite = {" O", "/|\\", "/ \\"};
                initCharacter(hero, level.startX, level.startY, heroSprite);

                bool levelCompleted = false;
                while (!levelCompleted) {
                    hero.updateNearestEnemy(level.enemies); // Обновляем ближайшего врага

                    handleInput(hero, level);
                    if(hero.wantToExit){
                        gameRunning = false;
                        break;
                    }
                    updateCharacterState(hero);

                // Проверка таймеров и обновление анимаций
                    if (hero.isPunching && hero.attackTimer.isExpired()) {
                        hero.isPunching = false;
                    }
                    if (hero.isJumping && hero.jumpTimer.isExpired()) {
                        hero.isJumping = false;
                    }

                    restoreScreen(hero, level.map);
                    drawCharacter(hero);
                    for (auto& enemy : level.enemies) {
                        enemy.offsetX = hero.offsetX;
                        enemy.offsetY = hero.offsetY;
                    }
                    //saveScreen();
                    
                    
                    updateEnemies(level.enemies, hero, level.map);
                    displayStats(hero);

                    if (hero.x == level.endX && hero.y == level.endY) {
                        levelCompleted = true;
                    }
                    if (hero.health <= 0) {
                        gameRunning = false;
                        break;
                    }
                }

                
                // if (!gameRunning || currentLevelIndex >= LevelManager::countLevels()) {
                //     gameRunning = false;
                //     break;
                // }
                if (!gameRunning) {
                    LevelManager::clearDirectory("levels/levelsGenerator/generatedLevels");
                    break;
                }
                currentLevelIndex++;
                LevelManager::generateNewLevel();
                LevelManager::loadLevel("levels/levelsGenerator/generatedLevels"); // Загружаем уровень

            }
        }
    }


   
}