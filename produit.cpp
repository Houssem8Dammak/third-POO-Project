#include "produit.h"
#include <iostream>

Produit::Produit(int id, const std::string& nom, const std::string& categorie, float prix, int qte){
    this->idProduit = id;
    this->nomProduit = nom;
    this->categorie = categorie;
    this->prix = prix;
    this->qteStock = qte;
}
int Produit::getId()const{return this->idProduit;}
const std::string& Produit::getNom()const{return this->nomProduit;}
const std::string& Produit::getCategorie()const{return this->categorie;}
float Produit::getPrix()const{return this->prix;}
int Produit::getQte()const{return this->qteStock;}
void Produit::setQte(int newQte){this->qteStock = newQte;}
void Produit::print()const{
    std::cout << "id produit: " << this->idProduit << " | "
    << "nom produit: " << this->nomProduit << " | "
    << "categorie: " << this->categorie << " | "
    << "prix de produit: " << this->prix  << " | "
    << "quantite en stock: " << this->qteStock << std::endl;
}