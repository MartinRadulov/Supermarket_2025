#pragma once

#include "utils.h"
#include <iostream>
#include <fstream>

class Category
{
private:
    int categoryId;
    char name[50];
    char description[200];

public:
    Category();
    Category(int catId, const char* catName, const char* catDescription);

    int getId() const
    {
        return categoryId;
    }
    const char* getName() const
    {
        return name;
    }
    const char* getDescription() const
    {
        return description;
    }

    void setName(const char* newName);
    void setDescription(const char* newDescription);

    void displayInfo() const;
    void saveToFile(std::ofstream& file) const;
    void loadFromFile(std::ifstream& file);

    static int generateNewCategoryId();
};

class Product
{
protected:
    int productId;
    char name[100];
    int categoryId;
    double price;

public:
    Product();
    Product(int prodId, const char* prodName, int catId, double prodPrice);

    virtual ~Product();

    virtual const char* getType() const = 0;
    virtual void displayInfo() const = 0;
    virtual void updateQuantity(double soldAmount) = 0;
    virtual double getAvailableQuantity() const = 0;
    virtual bool hasEnoughQuantity(double requestedAmount) const = 0;

    virtual void saveToFile(std::ofstream& file) const;
    virtual void loadFromFile(std::ifstream& file);

    int getId() const
    {
        return productId;
    }
    const char* getName() const
    {
        return name;
    }
    int getCategoryId() const
    {
        return categoryId;
    }
    double getPrice() const
    {
        return price;
    }

    void setName(const char* newName);
    void setPrice(double newPrice);
    void setCategoryId(int newCategoryId);

    static int generateNewProductId();
};

class ProductByUnit : public Product
{
private:
    int quantityUnits;

public:
    ProductByUnit();
    ProductByUnit(int prodId, const char* prodName, int catId, double prodPrice, int units);

    virtual ~ProductByUnit() = default;

    virtual const char* getType() const override;
    virtual void displayInfo() const override;
    virtual void updateQuantity(double soldAmount) override;
    virtual double getAvailableQuantity() const override;
    virtual bool hasEnoughQuantity(double requestedAmount) const override;

    virtual void saveToFile(std::ofstream& file) const override;
    virtual void loadFromFile(std::ifstream& file) override;

    int getQuantityUnits() const
    {
        return quantityUnits;
    }
    void setQuantityUnits(int units)
    {
        quantityUnits = units;
    }
    void addUnits(int additionalUnits)
    {
        quantityUnits += additionalUnits;
    }
};

class ProductByWeight : public Product
{
private:
    double quantityKg;

public:
    ProductByWeight();
    ProductByWeight(int prodId, const char* prodName, int catId, double prodPrice, double kg);

    virtual ~ProductByWeight() = default;

    virtual const char* getType() const override;
    virtual void displayInfo() const override;
    virtual void updateQuantity(double soldAmount) override;
    virtual double getAvailableQuantity() const override;
    virtual bool hasEnoughQuantity(double requestedAmount) const override;

    virtual void saveToFile(std::ofstream& file) const override;
    virtual void loadFromFile(std::ifstream& file) override;

    double getQuantityKg() const
    {
        return quantityKg;
    }
    void setQuantityKg(double kg)
    {
        quantityKg = kg;
    }
    void addKg(double additionalKg)
    {
        quantityKg += additionalKg;
    }
};

class ProductManager
{
private:
    Product** products;
    int productCount;
    int productCapacity;

    Category** categories;
    int categoryCount;
    int categoryCapacity;

public:
    ProductManager();
    ~ProductManager();

    bool addCategory(Category* category);
    Category* findCategoryById(int id);
    Category* findCategoryByName(const char* name);
    bool removeCategoryById(int id);
    void listAllCategories() const;
    bool hasCategoryProducts(int categoryId) const;

    bool addProduct(Product* product);
    Product* findProductById(int id) const;
    Product* findProductByName(const char* name);
    bool removeProductById(int id);
    void listAllProducts() const;
    void listProductsByCategory(int categoryId) const;

    bool updateProductStock(int productId, double amount, bool isAddition);
    bool hasEnoughStock(int productId, double requestedAmount) const;

    void saveAllCategories(const char* filename) const;
    void loadAllCategories(const char* filename);
    void saveAllProducts(const char* filename) const;
    void loadAllProducts(const char* filename);
    bool loadProductsFromReceipt(const char* filename);

    int getProductCount() const
    {
        return productCount;
    }
    int getCategoryCount() const
    {
        return categoryCount;
    }

private:
    void resizeProductArray();
    void resizeCategoryArray();
    void clearProducts();
    void clearCategories();
};
