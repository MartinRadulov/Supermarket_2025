#include "product.h"

Category::Category()
{
    categoryId = 0;
    strCopy(name, "", 50);
    strCopy(description, "", 200);
}

Category::Category(int catId, const char* catName, const char* catDescription)
{
    categoryId = catId;
    strCopy(name, catName, 50);
    strCopy(description, catDescription, 200);
}

void Category::setName(const char* newName)
{
    strCopy(name, newName, 50);
}

void Category::setDescription(const char* newDescription)
{
    strCopy(description, newDescription, 200);
}

void Category::displayInfo() const
{
    std::cout << "Category ID: " << categoryId << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Description: " << description << std::endl;
}

void Category::saveToFile(std::ofstream& file) const
{
    file << "CATEGORY:" << categoryId << ":" << name << ":" << description << std::endl;
}

void Category::loadFromFile(std::ifstream& file)
{
    char line[500];
    file.getline(line, 500);

    char* token = line;

    while (*token != ':' && *token != '\0') token++;
    if (*token == ':') token++;

    categoryId = 0;
    while (isDigitChar(*token))
    {
        categoryId = categoryId * 10 + (*token - '0');
        token++;
    }
    if (*token == ':') token++;

    char* nameStart = token;
    while (*token != ':' && *token != '\0') token++;
    *token = '\0';
    strCopy(name, nameStart, 50);
    token++;

    strCopy(description, token, 200);
}

int Category::generateNewCategoryId()
{
    static int nextCategoryId = 1;
    return nextCategoryId++;
}

Product::Product()
{
    productId = 0;
    strCopy(name, "", 100);
    categoryId = 0;
    price = 0.0;
}

Product::Product(int prodId, const char* prodName, int catId, double prodPrice)
{
    productId = prodId;
    strCopy(name, prodName, 100);
    categoryId = catId;
    price = prodPrice;
}

Product::~Product()
{

}

void Product::setName(const char* newName)
{
    strCopy(name, newName, 100);
}

void Product::setPrice(double newPrice)
{
    price = newPrice;
}

void Product::setCategoryId(int newCategoryId)
{
    categoryId = newCategoryId;
}

void Product::saveToFile(std::ofstream& file) const
{
    file << "PRODUCT:" << productId << ":" << name << ":" << categoryId << ":" << price;
}

void Product::loadFromFile(std::ifstream& file)
{
    char line[300];
    file.getline(line, 300);

    char* token = line;

    while (*token != ':' && *token != '\0') token++;
    if (*token == ':') token++;

    productId = 0;
    while (isDigitChar(*token))
    {
        productId = productId * 10 + (*token - '0');
        token++;
    }
    if (*token == ':') token++;

    char* nameStart = token;
    while (*token != ':' && *token != '\0') token++;
    *token = '\0';
    strCopy(name, nameStart, 100);
    token++;

    categoryId = 0;
    while (isDigitChar(*token))
    {
        categoryId = categoryId * 10 + (*token - '0');
        token++;
    }
    if (*token == ':') token++;

    price = 0.0;
    double multiplier = 1.0;
    bool foundDot = false;

    while (isDigitChar(*token) || *token == '.')
    {
        if (*token == '.') {
            foundDot = true;
            multiplier = 0.1;
        }
        else {
            if (!foundDot) {
                price = price * 10 + (*token - '0');
            }
            else {
                price += (*token - '0') * multiplier;
                multiplier *= 0.1;
            }
        }
        token++;
    }
}

int Product::generateNewProductId()
{
    static int nextProductId = 1001;
    return nextProductId++;
}

ProductByUnit::ProductByUnit() : Product()
{
    quantityUnits = 0;
}

ProductByUnit::ProductByUnit(int prodId, const char* prodName, int catId, double prodPrice, int units)
    : Product(prodId, prodName, catId, prodPrice)
{
    quantityUnits = units;
}

const char* ProductByUnit::getType() const
{
    return "PRODUCT_UNIT";
}

void ProductByUnit::displayInfo() const
{
    std::cout << productId << ". " << name << " : " << price << " : " << quantityUnits << std::endl;
}

void ProductByUnit::updateQuantity(double soldAmount)
{
    int soldUnits = (int)soldAmount;
    quantityUnits -= soldUnits;
    if (quantityUnits < 0) {
        quantityUnits = 0;
    }
}

double ProductByUnit::getAvailableQuantity() const
{
    return (double)quantityUnits;
}

bool ProductByUnit::hasEnoughQuantity(double requestedAmount) const
{
    int requestedUnits = (int)requestedAmount;
    return quantityUnits >= requestedUnits;
}

void ProductByUnit::saveToFile(std::ofstream& file) const
{
    file << "PRODUCT_UNIT:" << productId << ":" << name << ":" << categoryId << ":" << price << ":" << quantityUnits << std::endl;
}

void ProductByUnit::loadFromFile(std::ifstream& file)
{
    char line[300];
    file.getline(line, 300);

    char* token = line;

    while (*token != ':' && *token != '\0') token++;
    if (*token == ':') token++;

    productId = 0;
    while (isDigitChar(*token))
    {
        productId = productId * 10 + (*token - '0');
        token++;
    }
    if (*token == ':') token++;

    char* nameStart = token;
    while (*token != ':' && *token != '\0') token++;
    *token = '\0';
    strCopy(name, nameStart, 100);
    token++;

    categoryId = 0;
    while (isDigitChar(*token)) {
        categoryId = categoryId * 10 + (*token - '0');
        token++;
    }
    if (*token == ':') token++;

    price = 0.0;
    double multiplier = 1.0;
    bool foundDot = false;

    while (isDigitChar(*token) || *token == '.')
    {
        if (*token == '.')
        {
            foundDot = true;
            multiplier = 0.1;
        }
        else
        {
            if (!foundDot)
            {
                price = price * 10 + (*token - '0');
            }
            else
            {
                price += (*token - '0') * multiplier;
                multiplier *= 0.1;
            }
        }
        token++;
    }
    if (*token == ':') token++;

    quantityUnits = 0;
    while (isDigitChar(*token))
    {
        quantityUnits = quantityUnits * 10 + (*token - '0');
        token++;
    }
}


ProductByWeight::ProductByWeight() : Product()
{
    quantityKg = 0.0;
}

ProductByWeight::ProductByWeight(int prodId, const char* prodName, int catId, double prodPrice, double kg)
    : Product(prodId, prodName, catId, prodPrice)
{
    quantityKg = kg;
}

const char* ProductByWeight::getType() const
{
    return "PRODUCT_WEIGHT";
}

void ProductByWeight::displayInfo() const
{
    std::cout << productId << ". " << name << " : " << price << "/kg : " << quantityKg << std::endl;
}

void ProductByWeight::updateQuantity(double soldAmount)
{
    quantityKg -= soldAmount;
    if (quantityKg < 0.0) {
        quantityKg = 0.0;
    }
}

double ProductByWeight::getAvailableQuantity() const
{
    return quantityKg;
}

bool ProductByWeight::hasEnoughQuantity(double requestedAmount) const
{
    return quantityKg >= requestedAmount;
}

void ProductByWeight::saveToFile(std::ofstream& file) const
{
    file << "PRODUCT_WEIGHT:" << productId << ":" << name << ":" << categoryId << ":" << price << ":" << quantityKg << std::endl;
}

void ProductByWeight::loadFromFile(std::ifstream& file)
{
    char line[300];
    file.getline(line, 300);

    char* token = line;

    while (*token != ':' && *token != '\0') token++;
    if (*token == ':') token++;

    productId = 0;
    while (isDigitChar(*token))
    {
        productId = productId * 10 + (*token - '0');
        token++;
    }
    if (*token == ':') token++;

    char* nameStart = token;
    while (*token != ':' && *token != '\0') token++;
    *token = '\0';
    strCopy(name, nameStart, 100);
    token++;

    categoryId = 0;
    while (isDigitChar(*token)) {
        categoryId = categoryId * 10 + (*token - '0');
        token++;
    }
    if (*token == ':') token++;

    price = 0.0;
    double multiplier = 1.0;
    bool foundDot = false;

    while (isDigitChar(*token) || *token == '.')
    {
        if (*token == '.')
        {
            foundDot = true;
            multiplier = 0.1;
        }
        else
        {
            if (!foundDot)
            {
                price = price * 10 + (*token - '0');
            }
            else
            {
                price += (*token - '0') * multiplier;
                multiplier *= 0.1;
            }
        }
        token++;
    }
    if (*token == ':') token++;

    quantityKg = 0.0;
    multiplier = 1.0;
    foundDot = false;

    while (isDigitChar(*token) || *token == '.')
    {
        if (*token == '.')
        {
            foundDot = true;
            multiplier = 0.1;
        }
        else
        {
            if (!foundDot)
            {
                quantityKg = quantityKg * 10 + (*token - '0');
            }
            else 
            {
                quantityKg += (*token - '0') * multiplier;
                multiplier *= 0.1;
            }
        }
        token++;
    }
}

ProductManager::ProductManager()
{
    products = nullptr;
    productCount = 0;
    productCapacity = 0;

    categories = nullptr;
    categoryCount = 0;
    categoryCapacity = 0;
}

ProductManager::~ProductManager()
{
    clearProducts();
    clearCategories();
}

bool ProductManager::addCategory(Category* category)
{
    if (!category) return false;

    if (findCategoryByName(category->getName()))
    {
        return false;
    }

    if (categoryCount >= categoryCapacity)
    {
        resizeCategoryArray();
    }

    categories[categoryCount] = category;
    categoryCount++;
    return true;
}

Category* ProductManager::findCategoryById(int id)
{
    for (int i = 0; i < categoryCount; i++)
    {
        if (categories[i] && categories[i]->getId() == id)
        {
            return categories[i];
        }
    }
    return nullptr;
}

Category* ProductManager::findCategoryByName(const char* name)
{
    for (int i = 0; i < categoryCount; i++)
    {
        if (categories[i] && strCompare(categories[i]->getName(), name))
        {
            return categories[i];
        }
    }
    return nullptr;
}

bool ProductManager::removeCategoryById(int id)
{
    if (hasCategoryProducts(id))
    {
        return false;
    }

    for (int i = 0; i < categoryCount; i++)
    {
        if (categories[i] && categories[i]->getId() == id)
        {
            delete categories[i];

            for (int j = i; j < categoryCount - 1; j++)
            {
                categories[j] = categories[j + 1];
            }
            categoryCount--;
            return true;
        }
    }
    return false;
}

void ProductManager::listAllCategories() const
{
    std::cout << "=== CATEGORIES ===" << std::endl;
    for (int i = 0; i < categoryCount; i++)
    {
        if (categories[i])
        {
            categories[i]->displayInfo();
            std::cout << "---" << std::endl;
        }
    }
}

bool ProductManager::hasCategoryProducts(int categoryId) const
{
    for (int i = 0; i < productCount; i++)
    {
        if (products[i] && products[i]->getCategoryId() == categoryId)
        {
            return true;
        }
    }
    return false;
}

bool ProductManager::addProduct(Product* product)
{
    if (!product) return false;

    if (productCount >= productCapacity)
    {
        resizeProductArray();
    }

    products[productCount] = product;
    productCount++;
    return true;
}

Product* ProductManager::findProductById(int id) const
{
    for (int i = 0; i < productCount; i++)
    {
        if (products[i] && products[i]->getId() == id)
        {
            return products[i];
        }
    }
    return nullptr;
}

Product* ProductManager::findProductByName(const char* name)
{
    for (int i = 0; i < productCount; i++)
    {
        if (products[i] && strCompare(products[i]->getName(), name))
        {
            return products[i];
        }
    }
    return nullptr;
}

bool ProductManager::removeProductById(int id)
{
    for (int i = 0; i < productCount; i++)
    {
        if (products[i] && products[i]->getId() == id)
        {
            delete products[i];

            for (int j = i; j < productCount - 1; j++)
            {
                products[j] = products[j + 1];
            }
            productCount--;
            return true;
        }
    }
    return false;
}

void ProductManager::listAllProducts() const
{
    std::cout << "Products:" << std::endl;
    for (int i = 0; i < productCount; i++)
    {
        if (products[i])
        {
            products[i]->displayInfo();
        }
    }
}

void ProductManager::listProductsByCategory(int categoryId) const
{
    std::cout << "Products in category " << categoryId << ":" << std::endl;
    bool found = false;

    for (int i = 0; i < productCount; i++)
    {
        if (products[i] && products[i]->getCategoryId() == categoryId)
        {
            products[i]->displayInfo();
            found = true;
        }
    }

    if (!found)
    {
        std::cout << "No products found in this category" << std::endl;
    }
}

bool ProductManager::updateProductStock(int productId, double amount, bool isAddition)
{
    Product* product = findProductById(productId);
    if (!product) return false;

    if (isAddition) {
        ProductByUnit* unitProduct = dynamic_cast<ProductByUnit*>(product);
        if (unitProduct)
        {
            unitProduct->addUnits((int)amount);
            return true;
        }

        ProductByWeight* weightProduct = dynamic_cast<ProductByWeight*>(product);
        if (weightProduct)
        {
            weightProduct->addKg(amount);
            return true;
        }
    }
    else
    {
        if (product->hasEnoughQuantity(amount))
        {
            product->updateQuantity(amount);
            return true;
        }
    }

    return false;
}

bool ProductManager::hasEnoughStock(int productId, double requestedAmount) const
{
    Product* product = findProductById(productId);
    if (!product) return false;

    return product->hasEnoughQuantity(requestedAmount);
}

// File Operations
void ProductManager::saveAllCategories(const char* filename) const
{
    std::ofstream file(filename);
    if (!file.is_open()) return;

    for (int i = 0; i < categoryCount; i++) {
        if (categories[i]) {
            categories[i]->saveToFile(file);
        }
    }

    file.close();
}

void ProductManager::loadAllCategories(const char* filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) return;

    char line[500];
    while (file.getline(line, 500))
    {
        if (line[0] != '\0') {
            Category* category = new Category();

            char* token = line;

            while (*token != ':' && *token != '\0') token++;
            if (*token == ':') token++;

            int cat_id = 0;
            while (isDigitChar(*token))
            {
                cat_id = cat_id * 10 + (*token - '0');
                token++;
            }
            if (*token == ':') token++;

            char* nameStart = token;
            while (*token != ':' && *token != '\0') token++;
            char name[50];
            strCopy(name, nameStart, token - nameStart + 1);
            name[token - nameStart] = '\0';
            token++;

            char description[200];
            strCopy(description, token, 200);

            delete category;
            category = new Category(cat_id, name, description);
            addCategory(category);
        }
    }

    file.close();
}

void ProductManager::saveAllProducts(const char* filename) const
{
    std::ofstream file(filename);
    if (!file.is_open()) return;

    for (int i = 0; i < productCount; i++)
    {
        if (products[i])
        {
            products[i]->saveToFile(file);
        }
    }

    file.close();
}

void ProductManager::loadAllProducts(const char* filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) return;

    char line[300];
    while (file.getline(line, 300))
    {
        if (line[0] != '\0') {
            if (line[0] == 'P' && line[8] == 'U')
            { // PRODUCT_UNIT
                char* token = line;

                while (*token != ':' && *token != '\0') token++;
                if (*token == ':') token++;

                int prodId = 0;
                while (isDigitChar(*token))
                {
                    prodId = prodId * 10 + (*token - '0');
                    token++;
                }
                if (*token == ':') token++;

                char* nameStart = token;
                while (*token != ':' && *token != '\0') token++;
                *token = '\0';
                char prodName[100];
                strCopy(prodName, nameStart, 100);
                token++;

                int catId = 0;
                while (isDigitChar(*token))
                {
                    catId = catId * 10 + (*token - '0');
                    token++;
                }
                if (*token == ':') token++;

                double prodPrice = 0.0;
                double multiplier = 1.0;
                bool foundDot = false;
                while (isDigitChar(*token) || *token == '.')
                {
                    if (*token == '.')
                    {
                        foundDot = true;
                        multiplier = 0.1;
                    }
                    else {
                        if (!foundDot)
                        {
                            prodPrice = prodPrice * 10 + (*token - '0');
                        }
                        else
                        {
                            prodPrice += (*token - '0') * multiplier;
                            multiplier *= 0.1;
                        }
                    }
                    token++;
                }
                if (*token == ':') token++;

                int units = 0;
                while (isDigitChar(*token)) {
                    units = units * 10 + (*token - '0');
                    token++;
                }

                ProductByUnit* product = new ProductByUnit(prodId, prodName, catId, prodPrice, units);
                addProduct(product);

            }
            else if (line[0] == 'P' && line[8] == 'W')
            { // PRODUCT_WEIGHT
                char* token = line;

                while (*token != ':' && *token != '\0') token++;
                if (*token == ':') token++;

                int prodId = 0;
                while (isDigitChar(*token))
                {
                    prodId = prodId * 10 + (*token - '0');
                    token++;
                }
                if (*token == ':') token++;

                char* nameStart = token;
                while (*token != ':' && *token != '\0') token++;
                *token = '\0';
                char prodName[100];
                strCopy(prodName, nameStart, 100);
                token++;

                int catId = 0;
                while (isDigitChar(*token))
                {
                    catId = catId * 10 + (*token - '0');
                    token++;
                }
                if (*token == ':') token++;

                double prodPrice = 0.0;
                double multiplier = 1.0;
                bool foundDot = false;
                while (isDigitChar(*token) || *token == '.')
                {
                    if (*token == '.')
                    {
                        foundDot = true;
                        multiplier = 0.1;
                    }
                    else
                    {
                        if (!foundDot)
                        {
                            prodPrice = prodPrice * 10 + (*token - '0');
                        }
                        else
                        {
                            prodPrice += (*token - '0') * multiplier;
                            multiplier *= 0.1;
                        }
                    }
                    token++;
                }
                if (*token == ':') token++;

                double kg = 0.0;
                multiplier = 1.0;
                foundDot = false;
                while (isDigitChar(*token) || *token == '.')
                {
                    if (*token == '.')
                    {
                        foundDot = true;
                        multiplier = 0.1;
                    }
                    else
                    {
                        if (!foundDot)
                        {
                            kg = kg * 10 + (*token - '0');
                        }
                        else
                        {
                            kg += (*token - '0') * multiplier;
                            multiplier *= 0.1;
                        }
                    }
                    token++;
                }

                ProductByWeight* product = new ProductByWeight(prodId, prodName, catId, prodPrice, kg);
                addProduct(product);
            }
        }
    }

    file.close();
}

void ProductManager::loadProductsFromReceipt(const char* filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) return;

    char line[300];
    while (file.getline(line, 300))
    {
        if (line[0] != '\0')
        {

            if (line[0] == 'N' && line[1] == 'E' && line[2] == 'W')
            {
                // Ako e nujno
                continue;
            }
            else
            {
                char* token = line;

                while (*token != '>' && *token != '\0') token++;
                if (*token == '>') token++;
                if (*token == '>') token++;

                int prod_id = 0;
                while (isDigitChar(*token))
                {
                    prod_id = prod_id * 10 + (*token - '0');
                    token++;
                }
                if (*token == '>' && *(token + 1) == '>') token += 2;

                double quantity = 0.0;
                double multiplier = 1.0;
                bool foundDot = false;

                while (isDigitChar(*token) || *token == '.')
                {
                    if (*token == '.')
                    {
                        foundDot = true;
                        multiplier = 0.1;
                    }
                    else
                    {
                        if (!foundDot)
                        {
                            quantity = quantity * 10 + (*token - '0');
                        }
                        else
                        {
                            quantity += (*token - '0') * multiplier;
                            multiplier *= 0.1;
                        }
                    }
                    token++;
                }

                updateProductStock(prod_id, quantity, true);
            }
        }
    }

    file.close();
}

void ProductManager::resizeProductArray()
{
    int newCapacity = (productCapacity == 0) ? 2 : productCapacity * 2;
    Product** newArray = new Product * [newCapacity];

    for (int i = 0; i < productCount; i++)
    {
        newArray[i] = products[i];
    }

    delete[] products;
    products = newArray;
    productCapacity = newCapacity;
}

void ProductManager::resizeCategoryArray()
{
    int newCapacity = (categoryCapacity == 0) ? 2 : categoryCapacity * 2;
    Category** newArray = new Category * [newCapacity];

    for (int i = 0; i < categoryCount; i++)
    {
        newArray[i] = categories[i];
    }

    delete[] categories;
    categories = newArray;
    categoryCapacity = newCapacity;
}

void ProductManager::clearProducts()
{
    for (int i = 0; i < productCount; i++) {
        delete products[i];
    }
    delete[] products;
    products = nullptr;
    productCount = 0;
    productCapacity = 0;
}

void ProductManager::clearCategories()
{
    for (int i = 0; i < categoryCount; i++)
    {
        delete categories[i];
    }
    delete[] categories;
    categories = nullptr;
    categoryCount = 0;
    categoryCapacity = 0;
}