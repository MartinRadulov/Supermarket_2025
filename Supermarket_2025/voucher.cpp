#include "voucher.h"
#include <iostream>
#include <fstream>

template<>
int GiftCard<int>::counter = 0;

template<>
int GiftCard<int*>::counter = 0;

template<>
int GiftCard<double>::counter = 0;

template class GiftCard<int>;
template class GiftCard<int*>;
template class GiftCard<double>;

SingleCategoryGiftCard::SingleCategoryGiftCard(): GiftCard<int>(0), discountPercentage(0.0) {
}

SingleCategoryGiftCard::SingleCategoryGiftCard(int categoryId, double percentage)
    : GiftCard<int>(categoryId), discountPercentage(percentage) {
}

SingleCategoryGiftCard::SingleCategoryGiftCard(const SingleCategoryGiftCard& other)
    : GiftCard<int>(other.discountValue), discountPercentage(other.discountPercentage)
{
    strCopy(code, other.code, 6);
    isUsedV = other.isUsedV;
}

SingleCategoryGiftCard& SingleCategoryGiftCard::operator = (const SingleCategoryGiftCard& other)
{
    if (this != &other)
    {
        discountValue = other.discountValue;
        discountPercentage = other.discountPercentage;
        strCopy(code, other.code, 6);
        isUsedV = other.isUsedV;
    }
    return *this;
}

SingleCategoryGiftCard::SingleCategoryGiftCard(SingleCategoryGiftCard&& other) noexcept
    : GiftCard<int>(other.discountValue), discountPercentage(other.discountPercentage)
{
    strCopy(code, other.code, 6);
    isUsedV = other.isUsedV;

    other.discountValue = 0;
    other.discountPercentage = 0.0;
    other.isUsedV = false;
}

SingleCategoryGiftCard& SingleCategoryGiftCard::operator = (SingleCategoryGiftCard&& other) noexcept
{
    if (this != &other)
    {
        discountValue = other.discountValue;
        discountPercentage = other.discountPercentage;
        strCopy(code, other.code, 6);
        isUsedV = other.isUsedV;

        // Reset other object
        other.discountValue = 0;
        other.discountPercentage = 0.0;
        other.isUsedV = false;
    }
    return *this;
}

bool SingleCategoryGiftCard::canApplyToCategory(int categoryId) const
{
    return !isUsedV && (discountValue == categoryId);
}

double SingleCategoryGiftCard::calculateDiscount(double originalPrice, int categoryId) const
{
    if (!canApplyToCategory(categoryId))
    {
        return 0.0;
    }
    return originalPrice * (discountPercentage / 100.0);
}

const char* SingleCategoryGiftCard::getType() const
{
    return "SINGLE_CATEGORY";
}

void SingleCategoryGiftCard::displayInfo() const
{
    std::cout << "Single Category Gift Card\n";
    std::cout << "Code: " << code << "\n";
    std::cout << "Category ID: " << discountValue << "\n";
    std::cout << "Discount: " << discountPercentage << "%\n";
    std::cout << "Status: " << (isUsedV ? "USED" : "ACTIVE") << "\n";
}

void SingleCategoryGiftCard::saveToFile(std::ofstream& file) const
{
    file << "SINGLE_CATEGORY:" << discountValue << ":" << discountPercentage
        << ":" << code << ":" << (isUsedV ? 1 : 0) << "\n";
}

bool SingleCategoryGiftCard::loadFromFile(std::ifstream& file)
{
    char line[256];
    if (!file.getline(line, 256)) {
        return false;
    }

    SingleCategoryGiftCard* tempCard = GiftCardFactory::loadSingleFromString(line);

    if (!tempCard)
    {
        return false;
    }

    discountValue = tempCard->discountValue;
    discountPercentage = tempCard->discountPercentage;
    strCopy(code, tempCard->code, 6);
    isUsedV = tempCard->isUsedV;

    delete tempCard;

    return true;
}

MultipleCategoryGiftCard::MultipleCategoryGiftCard()
    : GiftCard<int*>(nullptr), categoryCount(0), categoryCapacity(4), discountPercentage(0.0)
{
    discountValue = new int[categoryCapacity];
}

MultipleCategoryGiftCard::MultipleCategoryGiftCard(const int* categoryIds, int count, double percentage)
    : GiftCard<int*>(nullptr), categoryCount(count), categoryCapacity(count + 4),
    discountPercentage(percentage)
{
    discountValue = new int[categoryCapacity];
    for (int i = 0; i < count; i++)
    {
        discountValue[i] = categoryIds[i];
    }
}

MultipleCategoryGiftCard::MultipleCategoryGiftCard(const MultipleCategoryGiftCard& other)
    : GiftCard<int*>(nullptr), categoryCount(other.categoryCount),
    categoryCapacity(other.categoryCapacity), discountPercentage(other.discountPercentage)
{
    copyCategoryArray(other);
    strCopy(code, other.code, 6);
    isUsedV = other.isUsedV;
}

MultipleCategoryGiftCard& MultipleCategoryGiftCard::operator = (const MultipleCategoryGiftCard& other)
{
    if (this != &other)
    {
        clearCategoryArray();
        categoryCount = other.categoryCount;
        categoryCapacity = other.categoryCapacity;
        discountPercentage = other.discountPercentage;
        copyCategoryArray(other);
        strCopy(code, other.code, 6);
        isUsedV = other.isUsedV;
    }
    return *this;
}

MultipleCategoryGiftCard::MultipleCategoryGiftCard(MultipleCategoryGiftCard&& other) noexcept
    : GiftCard<int*>(other.discountValue), categoryCount(other.categoryCount),
    categoryCapacity(other.categoryCapacity), discountPercentage(other.discountPercentage)
{
    strCopy(code, other.code, 6);
    isUsedV = other.isUsedV;

    other.discountValue = nullptr;
    other.categoryCount = 0;
    other.categoryCapacity = 0;
    other.discountPercentage = 0.0;
    other.isUsedV = false;
}

MultipleCategoryGiftCard& MultipleCategoryGiftCard::operator=(MultipleCategoryGiftCard&& other) noexcept
{
    if (this != &other)
    {
        clearCategoryArray();

        discountValue = other.discountValue;
        categoryCount = other.categoryCount;
        categoryCapacity = other.categoryCapacity;
        discountPercentage = other.discountPercentage;
        strCopy(code, other.code, 6);
        isUsedV = other.isUsedV;

        other.discountValue = nullptr;
        other.categoryCount = 0;
        other.categoryCapacity = 0;
        other.discountPercentage = 0.0;
        other.isUsedV = false;
    }
    return *this;
}

MultipleCategoryGiftCard::~MultipleCategoryGiftCard()
{
    clearCategoryArray();
}

void MultipleCategoryGiftCard::resizeCategoryArray()
{
    int newCapacity = categoryCapacity * 2;
    int* newArray = new int[newCapacity];

    for (int i = 0; i < categoryCount; i++)
    {
        newArray[i] = discountValue[i];
    }

    delete[] discountValue;
    discountValue = newArray;
    categoryCapacity = newCapacity;
}

void MultipleCategoryGiftCard::copyCategoryArray(const MultipleCategoryGiftCard& other)
{
    discountValue = new int[categoryCapacity];
    for (int i = 0; i < categoryCount; i++)
    {
        discountValue[i] = other.discountValue[i];
    }
}

void MultipleCategoryGiftCard::clearCategoryArray()
{
    delete[] discountValue;
    discountValue = nullptr;
}

bool MultipleCategoryGiftCard::canApplyToCategory(int categoryId) const
{
    if (isUsedV)
    {
        return false;
    }

    for (int i = 0; i < categoryCount; i++)
    {
        if (discountValue[i] == categoryId)
        {
            return true;
        }
    }
    return false;
}

double MultipleCategoryGiftCard::calculateDiscount(double originalPrice, int categoryId) const
{
    if (!canApplyToCategory(categoryId))
    {
        return 0.0;
    }
    return originalPrice * (discountPercentage / 100.0);
}

const char* MultipleCategoryGiftCard::getType() const
{
    return "MULTIPLE_CATEGORY";
}

void MultipleCategoryGiftCard::displayInfo() const
{
    std::cout << "Multiple Category Gift Card\n";
    std::cout << "Code: " << code << "\n";
    std::cout << "Categories: ";
    for (int i = 0; i < categoryCount; i++)
    {
        std::cout << discountValue[i];
        if (i < categoryCount - 1) std::cout << ", ";
    }
    std::cout << "\nDiscount: " << discountPercentage << "%\n";
    std::cout << "Status: " << (isUsedV ? "USED" : "ACTIVE") << "\n";
}

void MultipleCategoryGiftCard::addCategory(int categoryId)
{
    for (int i = 0; i < categoryCount; i++)
    {
        if (discountValue[i] == categoryId)
        {
            return;
        }
    }

    if (categoryCount >= categoryCapacity)
    {
        resizeCategoryArray();
    }

    discountValue[categoryCount++] = categoryId;
}

bool MultipleCategoryGiftCard::removeCategory(int categoryId)
{
    for (int i = 0; i < categoryCount; i++)
    {
        if (discountValue[i] == categoryId)
        {
            for (int j = i; j < categoryCount - 1; j++)
            {
                discountValue[j] = discountValue[j + 1];
            }
            categoryCount--;
            return true;
        }
    }
    return false;
}

void MultipleCategoryGiftCard::saveToFile(std::ofstream& file) const
{
    file << "MULTIPLE_CATEGORY:" << categoryCount;
    for (int i = 0; i < categoryCount; i++)
    {
        file << ":" << discountValue[i];
    }
    file << ":" << discountPercentage << ":" << code << ":" << (isUsedV ? 1 : 0) << "\n";
}

bool MultipleCategoryGiftCard::loadFromFile(std::ifstream& file)
{
    char line[512];
    if (!file.getline(line, 512)) {
        return false;
    }

    MultipleCategoryGiftCard* tempCard = GiftCardFactory::loadMultipleFromString(line);

    if (!tempCard)
    {
        return false;
    }

    clearCategoryArray();

    categoryCount = tempCard->categoryCount;
    categoryCapacity = tempCard->categoryCapacity;
    discountPercentage = tempCard->discountPercentage;
    strCopy(code, tempCard->code, 6);
    isUsedV = tempCard->isUsedV;

    discountValue = new int[categoryCapacity];
    for (int i = 0; i < categoryCount; i++)
    {
        discountValue[i] = tempCard->discountValue[i];
    }

    delete tempCard;

    return true;
}

AllProductsGiftCard::AllProductsGiftCard(): GiftCard<double>(0.0)
{
}

AllProductsGiftCard::AllProductsGiftCard(double percentage): GiftCard<double>(percentage)
{
}

AllProductsGiftCard::AllProductsGiftCard(const AllProductsGiftCard& other): GiftCard<double>(other.discountValue)
{
    strCopy(code, other.code, 6);
    isUsedV= other.isUsedV;
}

AllProductsGiftCard& AllProductsGiftCard::operator = (const AllProductsGiftCard& other)
{
    if (this != &other) {
        discountValue = other.discountValue;
        strCopy(code, other.code, 6);
        isUsedV = other.isUsedV;
    }
    return *this;
}

AllProductsGiftCard::AllProductsGiftCard(AllProductsGiftCard&& other) noexcept : GiftCard<double>(other.discountValue)
{
    strCopy(code, other.code, 6);
    isUsedV = other.isUsedV;

    other.discountValue = 0.0;
    other.isUsedV = false;
}

AllProductsGiftCard& AllProductsGiftCard::operator=(AllProductsGiftCard&& other) noexcept
{
    if (this != &other)
    {
        discountValue = other.discountValue;
        strCopy(code, other.code, 6);
        isUsedV = other.isUsedV;

        other.discountValue = 0.0;
        other.isUsedV = false;
    }
    return *this;
}

bool AllProductsGiftCard::canApplyToCategory(int category_id) const
{
    return !isUsedV;
}

double AllProductsGiftCard::calculateDiscount(double originalPrice, int categoryId) const
{
    if (isUsedV)
    {
        return 0.0;
    }
    return originalPrice * (discountValue / 100.0);
}

const char* AllProductsGiftCard::getType() const
{
    return "ALL_PRODUCTS";
}

void AllProductsGiftCard::displayInfo() const
{
    std::cout << "All Products Gift Card\n";
    std::cout << "Code: " << code << "\n";
    std::cout << "Discount: " << discountValue << "%\n";
    std::cout << "Status: " << (isUsedV ? "USED" : "ACTIVE") << "\n";
}

void AllProductsGiftCard::saveToFile(std::ofstream& file) const
{
    file << "ALL_PRODUCTS:" << discountValue << ":" << code << ":" << (isUsedV ? 1 : 0) << "\n";
}

bool AllProductsGiftCard::loadFromFile(std::ifstream& file)
{
    char line[256];
    if (!file.getline(line, 256))
    {
        return false;
    }

    AllProductsGiftCard* tempCard = GiftCardFactory::loadAllProductsFromString(line);

    if (!tempCard)
    {
        return false;
    }

    discountValue = tempCard->discountValue;
    strCopy(code, tempCard->code, 6);
    isUsedV = tempCard->isUsedV;

    delete tempCard;

    return true;
}

SingleCategoryGiftCard* GiftCardFactory::createSingleCategory(int categoryId, double percentage)
{
    return new SingleCategoryGiftCard(categoryId, percentage);
}

MultipleCategoryGiftCard* GiftCardFactory::createMultipleCategory(const int* categoryIds,
    int count, double percentage)
{
    return new MultipleCategoryGiftCard(categoryIds, count, percentage);
}

AllProductsGiftCard* GiftCardFactory::createAllProducts(double percentage)
{
    return new AllProductsGiftCard(percentage);
}

SingleCategoryGiftCard* GiftCardFactory::loadSingleFromString(const char* line) {

    const char* token = line;

    while (*token && *token != ':')
    {
        token++;
    }
    if (!*token)
    {
        return nullptr;
    }
    token++;

    char catStr[20];
    int i = 0;
    while (*token && *token != ':' && i < 19)
    {
        catStr[i++] = *token++;
    }
    catStr[i] = '\0';
    if (!*token) return nullptr;
    token++;

    char percentStr[20];
    i = 0;
    while (*token && *token != ':' && *token != '\0' && i < 19)
    {
        percentStr[i++] = *token++;
    }
    percentStr[i] = '\0';

    if (!isValidNumber(catStr) || !isValidFloat(percentStr))
    {
        return nullptr;
    }

    int categoryId = 0;
    for (int j = 0; catStr[j]; j++)
    {
        categoryId = categoryId * 10 + (catStr[j] - '0');
    }

    double percentage = 0.0;
    double multiplier = 1.0;
    bool afterDot = false;
    for (int j = 0; percentStr[j]; j++)
    {
        if (percentStr[j] == '.')
        {
            afterDot = true;
        }
        else
        {
            if (afterDot)
            {
                multiplier /= 10.0;
                percentage += (percentStr[j] - '0') * multiplier;
            }
            else
            {
                percentage = percentage * 10.0 + (percentStr[j] - '0');
            }
        }
    }

    return createSingleCategory(categoryId, percentage);
}

MultipleCategoryGiftCard* GiftCardFactory::loadMultipleFromString(const char* line) {
    const char* token = line;

    while (*token && *token != ':')
    {
        token++;
    }
    if (!*token)
    {
        return nullptr;
    }
    token++;

    char countStr[20];
    int i = 0;
    while (*token && *token != ':' && i < 19)
    {
        countStr[i++] = *token++;
    }
    countStr[i] = '\0';
    if (!*token)
    {
        return nullptr;
    }
    token++;

    if (!isValidNumber(countStr))
    {
        return nullptr;
    }

    int count = 0;
    for (int j = 0; countStr[j]; j++) {
        count = count * 10 + (countStr[j] - '0');
    }

    if (count <= 0 || count > 50)
    {
        return nullptr;
    }

    int* categories = new int[count];

    for (int catIdx = 0; catIdx < count; catIdx++)
    {
        char catStr[20];
        i = 0;
        while (*token && *token != ':' && i < 19)
        {
            catStr[i++] = *token++;
        }
        catStr[i] = '\0';

        if (!*token && catIdx < count - 1)
        {
            delete[] categories;
            return nullptr;
        }
        if (*token) token++;

        if (!isValidNumber(catStr))
        {
            delete[] categories;
            return nullptr;
        }

        categories[catIdx] = 0;
        for (int j = 0; catStr[j]; j++)
        {
            categories[catIdx] = categories[catIdx] * 10 + (catStr[j] - '0');
        }
    }

    char percentStr[20];
    i = 0;
    while (*token && *token != '\0' && i < 19)
    {
        percentStr[i++] = *token++;
    }
    percentStr[i] = '\0';

    if (!isValidFloat(percentStr))
    {
        delete[] categories;
        return nullptr;
    }

    double percentage = 0.0;
    double multiplier = 1.0;
    bool afterDot = false;
    for (int j = 0; percentStr[j]; j++)
    {
        if (percentStr[j] == '.')
        {
            afterDot = true;
        }
        else
        {
            if (afterDot)
            {
                multiplier /= 10.0;
                percentage += (percentStr[j] - '0') * multiplier;
            }
            else
            {
                percentage = percentage * 10.0 + (percentStr[j] - '0');
            }
        }
    }

    MultipleCategoryGiftCard* card = createMultipleCategory(categories, count, percentage);

    delete[] categories;

    return card;
}

AllProductsGiftCard* GiftCardFactory::loadAllProductsFromString(const char* line)
{
    const char* token = line;

    while (*token && *token != ':')
    {
        token++;
    }
    if (!*token)
    {
        return nullptr;
    }
    token++;

    char percentStr[20];
    int i = 0;
    while (*token && *token != '\0' && i < 19)
    {
        percentStr[i++] = *token++;
    }
    percentStr[i] = '\0';

    if (!isValidFloat(percentStr))
    {
        return nullptr;
    }

    double percentage = 0.0;
    double multiplier = 1.0;
    bool afterDot = false;
    for (int j = 0; percentStr[j]; j++)
    {
        if (percentStr[j] == '.')
        {
            afterDot = true;
        }
        else
        {
            if (afterDot)
            {
                multiplier /= 10.0;
                percentage += (percentStr[j] - '0') * multiplier;
            }
            else
            {
                percentage = percentage * 10.0 + (percentStr[j] - '0');
            }
        }
    }

    return createAllProducts(percentage);
}

void* GiftCardFactory::loadFromString(const char* line)
{
    if (!line || line[0] == '\0')
    {
        return nullptr;
    }

    if (line[0] == 'S' && line[1] == 'I')
    {
        return loadSingleFromString(line);
    }
    else if (line[0] == 'M' && line[1] == 'U')
    {
        return loadMultipleFromString(line);
    }
    else if (line[0] == 'A' && line[1] == 'L')
    {
        return loadAllProductsFromString(line);
    }

    return nullptr;
}
