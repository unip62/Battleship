#include "Client.hpp"
#include "PacketType.hpp"
#include <iostream>

Client::Client():_closeRunWait(false),posx(-1),posy(-1),res(false),_messageServeur("") {}

Client::~Client() {}

bool Client::getCloseRunWait() const{ return _closeRunWait;}
void Client::setCloseRunWait(bool etat) { _closeRunWait = etat;}
void Client::setBloquant(bool bloquant){
  _bloquant= bloquant;
}

bool Client::getJoueurIsActif() const{
  return _joueurIsActif;
}
void Client::setJoueurIsActif(bool joueurIsActif){
  _joueurIsActif=joueurIsActif;
}

sf::Socket::Status Client::connect(const sf::IpAddress & IP, unsigned short port)
{
  //connect to server
  sf::Socket::Status stat= _mySocket.connect(IP, port);
  _mySocket.setBlocking(false);//_bloquant);
  return stat;
}

sf::Socket::Status Client::send (PacketType type, const std::string & msg)
{
  sf::Packet packet;
  packet<<type<<msg;
  return _mySocket.send(packet);
}

sf::Socket::Status Client::send (sf::Packet & packet)
{
  return _mySocket.send(packet);
}

sf::Socket::Status Client::receive(std::string & msg)
{
  sf::Packet packet;
  sf::Socket::Status status=_mySocket.receive(packet);
  // PacketType type;
  //packet>>type;
  if(status==sf::Socket::Done)
    {
      msg = handlePackets(packet);
      std::cout<<msg<<"\n"; //messages en console si retour positioné dans handlePacket
      //packet>>msg;
    }
  return status;
}

std::string Client::handlePackets(sf::Packet & packet){
  PacketType type;
  std::string msg;
  std::string retour="";
  packet>>type;
  switch(type)
    {
    case SERVEUR_FULL:
      {
       packet>> _messageServeur;
      }
      break;

      case SERVEUR_MSG:
      {
       packet >> _messageServeur;
       retour = _messageServeur;
      }
      break;
      
    case DISCONNECT:
      {
	//_closeRunWait=true; 	
       packet >> _messageServeur;
       retour = _messageServeur;
       //ErrorMsg("Pseudo d�j� utilis� !", "Erreur pseudo");
       _closeRunWait=true;
      }
      break;
        
    case SEND_LISTE_ATTENTE:
      {
       packet>>msg;	
       _listeJoueurs = msg;
      }
      break;

    case SEND_USER_WAIT:
      {
	packet >> _messageServeur;
	setJoueurIsActif(false);
	std::cout << "msg : user Wait"<< _messageServeur << std::endl;
      }
      break;
      
    case START_PLAY:
      {
	packet>> _messageServeur;
	setJoueurIsActif(true);
	std::cout << "msg : starPlay"<< _messageServeur << std::endl;
      }
      break;

       case STOP_PLAY:
      {
	packet>> _messageServeur;
	setJoueurIsActif(false);
	std::cout << "msg : stopPlay"<< _messageServeur << std::endl;
      }
      break;

      case SEND_RESPONSE_COUP:
      {
	packet >> res >>posx >> posy;
	//_JoueurClt->setHitAt(r,x,y);
	//	setJoueurIsActif(false);
      }
      break;
      
    }
  return retour;
}



