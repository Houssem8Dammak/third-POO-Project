#ifndef PRODUIT_H
#define PRODUIT_H

#include <string>

class Produit{
private:
    int idProduit;
    std::string nomProduit;
    std::string categorie;
    float prix;
    int qteStock;
public:
    Produit(int id, const std::string& nom, const std::string& catgorie, float prix, int qte);
    int getId()const;
    const std::string& getNom() const;
    const std::string& getCategorie() const;
    float getPrix()const;
    int getQte()const;
    void setQte(int newQte);
    void print()const;

};



#endif