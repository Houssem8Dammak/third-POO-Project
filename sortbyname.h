#ifndef SORTBYNAME_H
#define SORTBYNAME_H

#include <vector>
#include <string>
#include "produit.h"

class SortByName{
public:
    bool operator()(const Produit& a, const Produit& b)const{
        return a.getNom() < b.getNom();
    }
};

#endif