#include "GameClient.hpp"
#include "Position.hpp"
#include "RandomInRange.hpp"
#include <iostream>
#include <sstream>
/*
 * Initialise simplement la fenetre avec une taille de 800*600,
 * un titre, et une impossibilité de resize.
 * La fenetre est ensuite centrée sur l'écran.
 */

GameClient::GameClient(const sf::Texture& bgPath) : _window(sf::VideoMode(800, 600),
				   "Battle Not Cheap - Ready to battle ? Let's connect !",
				   sf::Style::Titlebar | sf::Style::Close),
						    _wantsToPlay(true),
						    _sprBG(bgPath),
						    _joueur("")
{
	_window.setPosition(sf::Vector2i((sf::VideoMode::getDesktopMode().width-WINDOW_WIDTH)/2,
					 (sf::VideoMode::getDesktopMode().height-WINDOW_HEIGHT)/2 ));
}

/*
 * La première interface est le menu, le client
 * a la possibilité de quitter, ou de rejoindre 
 * un serveur.
 *
 */
GameClient::~GameClient() {}
void GameClient::run()
{
  std::string bufferIP = "IP Adress";
  std::string bufferPseudo = "Pseudo";
  std::string zoneSaisieTexte = "NothingForTheMoment";

  sf::Text connectText,quitText, saisieIP, saisiePseudo;

  sf::Sprite spr_zoneIP,spr_zonePseudo;
  sf::Texture txt_typeZone;

  sf::Font font;
  sf::Music music;
  
  unsigned int fieldMaxSize = 15;
  // Si le chargement des fichiers échoue
  if (!music.openFromFile("../Audio/menu.ogg"))
    exit(-1);
  if (!font.loadFromFile("../Fonts/DooM.ttf"))
    exit(-1);
  if (!txt_typeZone.loadFromFile("../Textures/typezone_bg.png"))
    exit(-1);

  // Initialisation des sprites & autres bidules
  connectText.setFont(font);
  connectText.setString("Connect now !");
  connectText.setCharacterSize(20);
  connectText.setColor(Black);
  connectText.setPosition(50,300);
  
  quitText.setFont(font);
  quitText.setString("Quit the game");
  quitText.setCharacterSize(20);
  quitText.setColor(Black);
  quitText.setPosition(50,350);

  saisiePseudo.setPosition(55, 205);
  saisiePseudo.setFont(font);
  saisiePseudo.setString(bufferPseudo);
  saisiePseudo.setCharacterSize(20);
  saisiePseudo.setColor(Gray);
  
  saisieIP.setPosition(55, 255);
  saisieIP.setFont(font);
  saisieIP.setString(bufferIP);
  saisieIP.setCharacterSize(20);
  saisieIP.setColor(Gray);

  spr_zonePseudo.setTexture(txt_typeZone);
  spr_zoneIP.setTexture(txt_typeZone);
  spr_zonePseudo.setPosition(50,200);  
  spr_zoneIP.setPosition(50,250);

  music.setVolume(40);
  music.play();

 
  while (_window.isOpen())
    {
      sf::Event event;
      while (_window.pollEvent(event))
	{
	  if (event.type == sf::Event::Closed)
	    {
	      _wantsToPlay=false;
	      _window.close();
	    }
	  if (event.type == sf::Event::MouseButtonPressed)
	    {
	      if (event.mouseButton.button == sf::Mouse::Left)
		{
		  // Si le client clique dans la zone d'un texte
		  if ( connectText.getGlobalBounds().contains(_window.mapPixelToCoords(sf::Mouse::getPosition(_window))) )
		    {
		      if ( _client.connect(bufferIP,5500) == sf::Socket::Done)
			{
			  _joueur.setPseudo(bufferPseudo);
			  music.stop();
			  if (_client.send(INITIAL_NAME_DATA, bufferPseudo)
			      == sf::Socket::Done ) {  
			    runWaitingRoom();}
			  }
			  
		    }
		  if ( quitText.getGlobalBounds().contains(_window.mapPixelToCoords(sf::Mouse::getPosition(_window))) )
		    {
		      _wantsToPlay=false;
		      _client.send(DISCONNECT, _joueur.getPseudo());
		      _window.close();
		    }
		  
		  if (spr_zonePseudo.getGlobalBounds().contains(_window.mapPixelToCoords(sf::Mouse::getPosition(_window))) )
		    {
		      saisiePseudo.setColor(DarkGray);
		      zoneSaisieTexte = "Pseudo";
		      saisieIP.setColor(Gray);
		    }
		  if (spr_zoneIP.getGlobalBounds().contains(_window.mapPixelToCoords(sf::Mouse::getPosition(_window))) )
		    {
		      if ( bufferIP == "IP Adress" )
			bufferIP = "";
		      saisieIP.setColor(DarkGray);
		      zoneSaisieTexte = "IP";
		      saisiePseudo.setColor(Gray);
		    }
		}
	    }
           if(event.type==sf::Event::TextEntered)
	     {
	       char code=static_cast<char>(event.text.unicode);
	       if (zoneSaisieTexte == "Pseudo")
		 { 
		   if( code != '\b' && bufferPseudo.size() < fieldMaxSize ) 
		     bufferPseudo.push_back(code);
		   else if( code == '\b' )
		     {
		       if(bufferPseudo.size() > 0) 
			 bufferPseudo.pop_back();
		     }
		   
		 } else if (zoneSaisieTexte == "IP")
		 {	 
		   if( code != '\b' && bufferIP.size() < fieldMaxSize ) 
		     bufferIP.push_back(code);
		   else if( code == '\b')
		     {
		       if(bufferIP.size()>0) 
			 bufferIP.pop_back();
		     }
		 }
	     }
	   
	   // Si la souris est dans la zone du texte
	   if ( connectText.getGlobalBounds().contains(_window.mapPixelToCoords(sf::Mouse::getPosition(_window))) )
	     connectText.setColor(RedWine);
	   if ( ! connectText.getGlobalBounds().contains(_window.mapPixelToCoords(sf::Mouse::getPosition(_window))) )
	     connectText.setColor(Black);
	   
	   if ( quitText.getGlobalBounds().contains(_window.mapPixelToCoords(sf::Mouse::getPosition(_window))) )
	     quitText.setColor(RedWine);
	   if ( ! quitText.getGlobalBounds().contains(_window.mapPixelToCoords(sf::Mouse::getPosition(_window))) )
	     quitText.setColor(Black);
	}
      
      _window.clear(White);
      
      _window.draw(_sprBG);
      
      saisiePseudo.setString(bufferPseudo);
      saisieIP.setString(bufferIP);
      
      _window.draw(spr_zoneIP);
      _window.draw(spr_zonePseudo);   
      
      _window.draw(saisieIP);
      _window.draw(saisiePseudo);
      _window.draw(connectText);
      _window.draw(quitText);
      
      _window.display();
    }
}

/*
 * La deuxième interface est la Waiting Room, le joueur
 * choisit sa flotte, et attend l'autre personne 
 * 
 * CF explications de run()
 *
 */

void GameClient::runWaitingRoom()
{
  _window.setTitle("Battle Not Cheap - Let's place your ships captain !");
  sf::Sprite spr_grid, spr_wlist,spr_radis;
  sf::Texture txt_grid, txt_wlist,txt_radis;
  sf::Text randomPosText, wlistText;
  sf::Font font;
  if (!font.loadFromFile("../Fonts/DooM.ttf"))
    exit(-1);
  if (!txt_grid.loadFromFile("../Textures/grid_bg.png"))
    exit(-1);
  if (!txt_wlist.loadFromFile("../Textures/waitinglist_bg.png"))
    exit(-1);
  if (!txt_radis.loadFromFile("../Textures/radis.png"))
    exit(-1);

  spr_grid.setTexture(txt_grid);
  spr_grid.setPosition(50,125);
  
  spr_wlist.setTexture(txt_wlist);
  spr_wlist.setPosition(450,200);

  spr_radis.setTexture(txt_radis);
  spr_radis.setPosition(700,525);

  randomPosText.setFont(font);
  randomPosText.setString("Generate random position");
  randomPosText.setCharacterSize(20);
  randomPosText.setColor(Black);
  randomPosText.setPosition(50,500);


  wlistText.setFont(font);
  wlistText.setString("");
  wlistText.setCharacterSize(20);
  wlistText.setColor(Black);
  wlistText.setPosition(460,220);

  _joueur.setRandFlotte();

  while (_window.isOpen())
    {
      sf::Event event;
      while (_window.pollEvent(event))
	{
	  if (event.type == sf::Event::Closed)
	    _window.close();
	  if (event.type == sf::Event::MouseButtonPressed)
	    {
	      if (event.mouseButton.button == sf::Mouse::Left)
		{
		  if ( spr_radis.getGlobalBounds().contains(_window.mapPixelToCoords(sf::Mouse::getPosition(_window))) )
		    { //eric
		      std::ostringstream uneflotte;
		      uneflotte << _joueur.getFlotte();
		      if (_client.send(SEND_FLOTTE, uneflotte.str())
			  == sf::Socket::Done ) {  
			runBoards();}
		    }
		 
		  if ( randomPosText.getGlobalBounds().contains(_window.mapPixelToCoords(sf::Mouse::getPosition(_window))) )
		    {
		      _joueur.setRandFlotte();
		      drawSpritesGrid(spr_grid.getPosition().x,spr_grid.getPosition().y);
		    }
		}
	    } 
	  if ( randomPosText.getGlobalBounds().contains(_window.mapPixelToCoords(sf::Mouse::getPosition(_window))) )
	    randomPosText.setColor(RedWine);
	  if ( ! randomPosText.getGlobalBounds().contains(_window.mapPixelToCoords(sf::Mouse::getPosition(_window))) )
	    randomPosText.setColor(Black);
	  if ( spr_radis.getGlobalBounds().contains(_window.mapPixelToCoords(sf::Mouse::getPosition(_window))) )
	    spr_radis.setColor(DarkGray);
	  if ( !spr_radis.getGlobalBounds().contains(_window.mapPixelToCoords(sf::Mouse::getPosition(_window))) )
	    spr_radis.setColor(White);
	}

      std::string msg="";
      if (_client.receive(msg) == sf::Socket::Done){
        wlistText.setString(_client._ListeJoueurs);    

      }
      
      _window.clear(White);
      _window.draw(_sprBG);
      _window.draw(spr_radis);
      _window.draw(spr_wlist);
      _window.draw(wlistText);
      _window.draw(spr_grid);
      _window.draw(randomPosText);
      drawSpritesGrid(spr_grid.getPosition().x,spr_grid.getPosition().y);
      _window.display();
    }
  
}

/*
 * La troisième interface est le jeu.
 *
 * CF explications de run()
 * 
 */

void GameClient::runBoards()
{
  _window.setTitle("Battle Not Cheap - FIRE !");
  
  sf::Sprite spr_grid, spr_grid2;
  sf::Texture txt_grid;
  
  if (!txt_grid.loadFromFile("../Textures/grid_bg.png"))
    exit(-1);
  
  spr_grid.setTexture(txt_grid);
  spr_grid.setPosition(25,125);
  
  spr_grid2.setTexture(txt_grid);
  spr_grid2.setPosition(425,125);
  
  while (_window.isOpen())
    {
      sf::Event event;
      while (_window.pollEvent(event))
	{
	  if (event.type == sf::Event::Closed)
	    _window.close();
	  if (event.type == sf::Event::MouseButtonPressed)
	    {
	      if (event.mouseButton.button == sf::Mouse::Left)
		{
		  if ( spr_grid2.getGlobalBounds().contains(_window.mapPixelToCoords(sf::Mouse::getPosition(_window))) )
		    {
		      // Echange client serveur A FAIRE ERIC //////////////////
		      int xPosInGrid2 = (sf::Mouse::getPosition(_window).x - 425)/CELL_SIZE;
		      int yPosInGrid2 = (sf::Mouse::getPosition(_window).y - 125)/CELL_SIZE;
		      RandomInRange r;
		      _joueur.setHitAt(r(1,2),xPosInGrid2,yPosInGrid2);
		    }
		}
	    }
	}
      
      _window.clear(White);
      _window.draw(_sprBG);
      _window.draw(spr_grid);
      _window.draw(spr_grid2);
      drawSpritesGrid(spr_grid.getPosition().x,spr_grid.getPosition().y);
      drawSpritesHits(spr_grid2.getPosition().x,spr_grid2.getPosition().y);
      _window.display();
    }
}

void GameClient::drawSpritesGrid(float posGridX, float posGridY)
{
  sf::Texture txt_intactCell,txt_seaCell;
  if (!txt_intactCell.loadFromFile("../Textures/ship_intact.png"))
    exit(-1);
  if (!txt_seaCell.loadFromFile("../Textures/sea_cell.png"))
    exit(-1);
  sf::Sprite spr_intactCell,spr_seaCell;
  spr_intactCell.setTexture(txt_intactCell);
  spr_seaCell.setTexture(txt_seaCell);
  
  for ( int x=0 ; x < 10 ; x++ )
    {
      for ( int y=0 ; y < 10 ; y++)
	{
	  if ( _joueur.getFlotte().foundInFlotte(Position{x,y}) )
	    {
	      spr_intactCell.setPosition((CELL_SIZE*x)+posGridX+2,(CELL_SIZE*y)+posGridY+2);
	      _window.draw(spr_intactCell);
	    }
	  else
	    {
	      spr_seaCell.setPosition((CELL_SIZE*x)+posGridX+2,(CELL_SIZE*y)+posGridY+2);
	      _window.draw(spr_seaCell);
	    }
	}
    }
}

void GameClient::drawSpritesHits(float posGridX, float posGridY)
{
  sf::Texture txt_missCell,txt_hitCell,txt_seaCell;
  if (!txt_missCell.loadFromFile("../Textures/ship_miss.png"))
    exit(-1);
  if (!txt_seaCell.loadFromFile("../Textures/sea_cell.png"))
    exit(-1);
  if (!txt_hitCell.loadFromFile("../Textures/ship_hit.png"))
    exit(-1);

  sf::Sprite spr_missCell,spr_seaCell,spr_hitCell;
  spr_missCell.setTexture(txt_missCell);
  spr_seaCell.setTexture(txt_seaCell);
  spr_hitCell.setTexture(txt_hitCell);

  for ( int x=0 ; x < 10 ; x++ )
    {
      for ( int y=0 ; y < 10 ; y++)
	{
	  if ( _joueur.getHitAt(x,y) == SEA_CELL )
	    {
	      spr_seaCell.setPosition((CELL_SIZE*x)+posGridX+2,(CELL_SIZE*y)+posGridY+2);
	      _window.draw(spr_seaCell);
	    }
	  else if ( _joueur.getHitAt(x,y) == MISS_CELL )
	    {
	      spr_missCell.setPosition((CELL_SIZE*x)+posGridX+2,(CELL_SIZE*y)+posGridY+2);
	      _window.draw(spr_missCell);
	    }
	  else
	    {
	      spr_hitCell.setPosition((CELL_SIZE*x)+posGridX+2,(CELL_SIZE*y)+posGridY+2);
	      _window.draw(spr_hitCell);
	    }
	}
    }
}

bool GameClient::getWantsToPlay() { return _wantsToPlay; }
