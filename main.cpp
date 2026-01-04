#include <iostream>
#include <list>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <algorithm>
#include "produit.h"
#include "sortbyname.h"
#include "sortbyprice.h"
#include "idcheck.h"

using namespace std;

void fileOpening(FILE **file,const char* openingMode){
    *file = fopen("./produit.txt",openingMode);
    if (!(*file)){
        cout << "Error in openening the file." << endl;
        exit(1);
    }
}

void chargingFromFile(FILE* file, vector<Produit> &vect){
    //opening file to read the data from it 
    fileOpening(&file,"r");
    //initializing the temporary varibales 
    char line[256];
    int id, qte;
    char nom[100], categorie[100];  // These are char arrays 
    float prix;
    
    while(fgets(line, 256, file)){
        if(sscanf(line, "%d:%99[^:]:%99[^:]:%f:%d", &id, nom, categorie, &prix, &qte) == 5){
            //emplace_back takes the argument that is supposed to be taken by the constructor and it constructs and adds it to the back of the vector (it combines constructor and .push_back)
            vect.emplace_back(id, std::string(nom), std::string(categorie), prix, qte);
        }
    }
    
    fclose(file);
}

void savingToFile(FILE* file,vector<Produit> &vect){
    fileOpening(&file,"w");
    for(const auto& element: vect){
        fprintf(file,"%d:%s:%s:%.2f:%d\n",element.getId(),element.getNom().c_str(),element.getCategorie().c_str(),element.getPrix(),element.getQte());
    }
    fclose(file);
}

void createList(vector<Produit>& vect,list<Produit*>& l){
    for(auto& product: vect){
        if(product.getQte()>0){
            l.push_back(&product);
        }
    }
}

void createMap(vector<Produit>& vect,map<int,Produit*>& m){
    for(auto& product: vect){
        m[product.getId()] = &product;
    }
}

bool isProductAlreadyExists(map<int, Produit*>& productMap, int id){
    auto it = productMap.find(id);
    
    if(it != productMap.end()){
        return true;
    } else {
        return false;
    }
}

void rebuildMap(vector<Produit>& vect, map<int,Produit*>& m){
    m.clear();
    createMap(vect, m);
}

Produit* findProductById(map<int, Produit*>& productMap, int id){
    auto it = productMap.find(id);
    if(it != productMap.end()){
        return it->second;
    }
    return nullptr;
}

void rebuildAvailableList(vector<Produit>& vect, list<Produit*>& l){
    l.clear();
    createList(vect, l);
}

void searchProductById(map<int, Produit*>& productMap){
    int searchId;
    cout << "Enter product ID to search: ";
    cin >> searchId;
    
    Produit* product = findProductById(productMap, searchId);
    
    if(product){
        cout << "\n--- PRODUCT FOUND ---" << endl;
        product->print();
        if(product->getQte() > 0){
            cout << "Status: AVAILABLE " << endl;
        } else {
            cout << "Status: OUT OF STOCK " << endl;
        }
    } else {
        cout << "\nProduct with ID " << searchId << " not found!" << endl;
    }
}

void addProduct(vector<Produit> &vect, list<Produit*>& l, const set<string>& categories,map<int,Produit*>& m){
    cout << "we will begin the inesrtion process" << endl;
    
    //this part reads the values we want to add
    int idBuffer{};
    do{
        cout << "insert the id of the product: " ;
        cin >> idBuffer;
    }while(isProductAlreadyExists(m,idBuffer));

    cout << "insert the name of the product: ";
    string nameBuffer{};
    cin >> nameBuffer;

    // Display available categories
    cout << "\nAvailable categories:" << endl;
    int i = 1;
    for(const auto& cat : categories){
        cout << i++ << ". " << cat << endl;
    }

    cout << "\ninsert the category of the product: ";
    string categoryBuffer{};
    cin >> categoryBuffer;

    // Validate category
    while(categories.find(categoryBuffer) == categories.end()){
        cout << "Invalid category! Please choose from the list above: ";
        cin >> categoryBuffer;
    }

    cout << "insert the price of the product:";
    float priceBuffer{};
    cin >> priceBuffer;

    cout << "insert the quantity of the product:";
    int qteBuffer{};
    cin >> qteBuffer;

    //constructs the object and adds it to the back of the vector
    vect.emplace_back(idBuffer,nameBuffer,categoryBuffer,priceBuffer,qteBuffer);
    //since the list contains ponietrs to the objects held by the vector so if the vector changes place in the memory the list doesn't get affected
    rebuildAvailableList(vect, l);
    rebuildMap(vect,m);
}

void removeProduct(vector<Produit> &vect, list<Produit*>& l,map<int,Produit*>& m){
    //inserting index of the product to be removed
    int index;
    do{
        cout << "choose at what index the product you want to remove:";
        cin >> index;
    }while(index <= 0 || (size_t)index > vect.size());

    int idRemove = vect.at(index-1).getId(); //saving the index to use it for the list removal
    
    l.erase(remove_if(l.begin(),l.end(),IdCheck(idRemove)),l.end());//removing from list
    
    vect.erase(vect.begin()+index-1);//removing from vector

    rebuildAvailableList(vect,l);
    rebuildMap(vect,m);
}

void printAllProducts(vector<Produit> &vect){
    for(Produit& element: vect){
        element.print();
    }
}

void sortingByName(vector<Produit>& vect){
    sort(vect.begin(),vect.end(),SortByName());
}

void sortingByPrice(vector<Produit>& vect){
    sort(vect.begin(),vect.end(),SortByPrice());
}

void printAvailableProducts(list<Produit*>& l){
    for(Produit* element : l){
        element->print();
    }
}

void sellingProduct(vector<Produit>& vect, list<Produit*>& l){
    int soldProductId;
    int soldProductQte;

    cout << "insert the id of the product you sold: ";
    cin >> soldProductId;

    auto it = find_if(vect.begin(),vect.end(),IdCheck(soldProductId));
    
    if (it == vect.end()) {
        cout << "Product not found!" << endl;
        return;
    }

    do{
        cout << "insert the quantity you sold: ";
        cin >> soldProductQte;
    }while(soldProductQte < 0 || ((it->getQte() - soldProductQte) < 0));

    //updates the value of qte in the vector
    int newQte {it->getQte() - soldProductQte};
    it->setQte(newQte);

    //updates the list
     if(newQte == 0){
        auto listIt = find_if(l.begin(), l.end(), IdCheck(soldProductId));
        if(listIt != l.end()){
            l.erase(listIt);
        }
    }
}

void restockingProduct(vector<Produit>& vect, list<Produit*>& l){
    int restockProductId, restockProductQte;
    cout << "insert the  id of the product you want to restock: ";
    cin >> restockProductId;

    do{
        cout << "insert the quantity to restock: ";
        cin >> restockProductQte;
    }while(restockProductQte < 0 );

    auto it = find_if(vect.begin(),vect.end(),IdCheck(restockProductId));

    if (it == vect.end()) {
        cout << "Product not found!" << endl;
        return;
    }

    int newQte {it->getQte() + restockProductQte};
    it->setQte(newQte);

    auto listIt = find_if(l.begin(),l.end(),IdCheck(restockProductId));
    if(listIt == l.end()){
        l.push_back(&(*it));
    }
}

int main(){
    //initialization
    vector<Produit> products{};
    FILE* file = nullptr;
    list<Produit*> availableProducts;
    set<string> validCategories = {
    "Electronics",
    "Furniture",
    "Accessories",
    "Clothing",
    "Food",
    "Books",
    "Sports"
    };
    map<int,Produit*> productsMap{};
    //reading from file and creating the vector
    chargingFromFile(file, products);
    
    //creating the available products list
    createList(products, availableProducts);

    //creating the products map
    createMap(products, productsMap);
    
    int choice;
    bool running = true;
    
    while(running){
        cout << "\n========== PRODUCT MANAGEMENT SYSTEM ==========" << endl;
        cout << "1. Display all products" << endl;
        cout << "2. Display available products (quantity > 0)" << endl;
        cout << "3. Add a new product" << endl;
        cout << "4. Remove a product" << endl;
        cout << "5. Sell a product" << endl;
        cout << "6. Restock a product" << endl;
        cout << "7. Sort products by name" << endl;
        cout << "8. Sort products by price" << endl;
        cout << "9. Search product by ID" << endl;
        cout << "10. Save and exit" << endl;
        cout << "===============================================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch(choice){
            case 1:
                cout << "\n--- ALL PRODUCTS ---" << endl;
                if(products.empty()){
                    cout << "No products in inventory." << endl;
                } else {
                    printAllProducts(products);
                }
                break;
                
            case 2:
                cout << "\n--- AVAILABLE PRODUCTS ---" << endl;
                if(availableProducts.empty()){
                    cout << "No available products (all out of stock)." << endl;
                } else {
                    printAvailableProducts(availableProducts);
                }
                break;
                
            case 3:
                cout << "\n--- ADD NEW PRODUCT ---" << endl;
                addProduct(products, availableProducts, validCategories, productsMap);
                cout << "Product added successfully!" << endl;
                break;
                
            case 4:
                cout << "\n--- REMOVE PRODUCT ---" << endl;
                if(products.empty()){
                    cout << "No products to remove." << endl;
                } else {
                    cout << "Current products:" << endl;
                    for(size_t i = 0; i < products.size(); i++){
                        cout << (i+1) << ". ID: " << products[i].getId() 
                             << " - " << products[i].getNom() << endl;
                    }
                    removeProduct(products, availableProducts, productsMap);
                    cout << "Product removed successfully!" << endl;
                }
                break;
                
            case 5:
                cout << "\n--- SELL PRODUCT ---" << endl;
                if(availableProducts.empty()){
                    cout << "No available products to sell." << endl;
                } else {
                    cout << "Available products:" << endl;
                    printAvailableProducts(availableProducts);
                    sellingProduct(products, availableProducts);
                    cout << "Sale completed!" << endl;
                }
                break;
                
            case 6:
                cout << "\n--- RESTOCK PRODUCT ---" << endl;
                if(products.empty()){
                    cout << "No products in inventory." << endl;
                } else {
                    cout << "Current products:" << endl;
                    for(const auto& p : products){
                        cout << "ID: " << p.getId() << " - " << p.getNom() 
                             << " (Current stock: " << p.getQte() << ")" << endl;
                    }
                    restockingProduct(products, availableProducts);
                    cout << "Product restocked successfully!" << endl;
                }
                break;
                
            case 7:
                cout << "\n--- SORTING BY NAME ---" << endl;
                sortingByName(products);
                rebuildAvailableList(products, availableProducts);
                rebuildMap(products, productsMap);
                cout << "Products sorted by name!" << endl;
                printAllProducts(products);
                break;
                
            case 8:
                cout << "\n--- SORTING BY PRICE ---" << endl;
                sortingByPrice(products);
                rebuildAvailableList(products, availableProducts);
                rebuildMap(products, productsMap);
                cout << "Products sorted by price!" << endl;
                printAllProducts(products);
                break;
            
            case 9:
                cout << "\n--- SEARCH PRODUCT BY ID ---" << endl;
                if(products.empty()){
                    cout << "No products in inventory." << endl;
                } else {
                    searchProductById(productsMap);
                }
                break;
                
            case 10:
                cout << "\n--- SAVING AND EXITING ---" << endl;
                savingToFile(file, products);
                cout << "Data saved successfully!" << endl;
                cout << "Goodbye!" << endl;
                running = false;
                break;
                
            default:
                cout << "Invalid choice! Please enter a number between 1 and 9." << endl;
                break;
        }
        
        // Pause to let user read output
        if(running && choice >= 1 && choice <= 9){
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }
    
    return 0;
}