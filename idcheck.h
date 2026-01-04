#ifndef IDCHECK_H
#define IDCHECK_H

#include "produit.h"

class IdCheck{
private:
    int targetId;
public:
    IdCheck(int id): targetId{id}{}

    bool operator()(Produit* element) const{
        return element->getId() == this->targetId;
    }
    bool operator()(const Produit& element) const{
        return element.getId() == this->targetId;
    }
};

#endif