#ifndef FLOTTE_HPP
#define FLOTTE_HPP

#include "Bateau.hpp"
#include <vector>

/*
 * Représente l'ensemble des bateaux
 *
 * ASKME: Baptiste FAMCHON
 */


class Flotte {
private:
  std::vector<Bateau> _bateaux;
  int _nbBateaux;

public:
  Flotte();
  void genererFlotte();
  int estValide(Position,int,int);
  bool estValideHaut(Position,int);
  bool estValideBas(Position,int,int);
  bool estValideGauche(Position,int);
  bool estValideDroite(Position,int,int);
  bool foundInFlotte(Position) const;
};


std::ostream & operator<<(std::ostream & os, const Flotte &);
#endif