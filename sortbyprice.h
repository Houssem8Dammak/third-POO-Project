#ifndef SORTBYPRICE_H
#define SORTBYPRICE_H

#include <vector>
#include <string>
#include "produit.h"

class SortByPrice{
public:
    bool operator()(const Produit& a, const Produit& b)const{
        return a.getPrix() < b.getPrix();
    }
};

#endif