#ifndef GAMECLIENT_HPP
#define GAMECLIENT_HPP

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>

#include "Flotte.hpp"

/*
 * Constantes utiles pour simplifier l'écriture
 * et la lecture du code.
 * Elles ne sont pas toutes utilisées pour le moment.
 *
 * ASKME: Baptiste FAMCHON
 */

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const int GRID_WIDTH = 350;
const int GRID_HEIGHT = 350;
const int CELL_SIZE = 35;
const int CELL_WIDTH = GRID_WIDTH / CELL_SIZE;
const int CELL_HEIGHT = GRID_HEIGHT / CELL_SIZE;

const sf::Color White (255,255,255);
const sf::Color Black (0,0,0);
const sf::Color Gray (240,240,240);
const sf::Color DarkGray (150,150,150);
const sf::Color RedWine (127,26,26);


class GameClient {
private:
  sf::RenderWindow _window;
  bool _wantsToPlay;
  Flotte* _flotte;

  void runWaitingRoom();
  void runBoards();
  void drawSpriteBG(std::string);
  void drawSpritesGrid();
public:
  GameClient();
  void run();
  bool getWantsToPlay();
};

#endif