#pragma once

#include "utils.h"
#include <fstream>

template<typename T>
class GiftCard
{
protected:
    char code[6];
    T discountValue;
    bool isUsed;
    static int counter;

    void generateCode() {
        counter++;
        code[0] = '0' + (counter % 10);
        code[1] = 'A' + (counter % 26);
        code[2] = '0' + ((counter / 10) % 10);
        code[3] = 'A' + ((counter / 26) % 26);
        code[4] = '0' + ((counter / 100) % 10);
        code[5] = '\0';
    }

public:
    GiftCard()
    {
        discountValue = T{};
        isUsed = false;
        generateCode();
    }

    GiftCard(const T& value)
    {
        discountValue = value;
        isUsed = false;
        generateCode();
    }

    virtual ~GiftCard() = default;

    virtual bool canApplyToCategory(int categoryId) const = 0;
    virtual double calculateDiscount(double originalPrice, int categoryId) const = 0;
    virtual const char* getType() const = 0;
    virtual void displayInfo() const = 0;

    const char* getCode() const
    {
        return code;
    }
    bool isUsed() const
    {
        return isUsed;
    }
    void markAsUsed()
    {
        isUsed = true;
    }

    void resetUsed()
    {
        isUsed = false;
    }

    virtual void saveToFile(std::ofstream& file) const = 0;
    virtual bool loadFromFile(std::ifstream& file) = 0;
};

template<typename T>
int GiftCard<T>::counter = 0;

class SingleCategoryGiftCard : public GiftCard<int>
{
private:
    double discountPercentage;

public:
    SingleCategoryGiftCard();
    SingleCategoryGiftCard(int categoryId, double percentage);

    SingleCategoryGiftCard(const SingleCategoryGiftCard& other);

    SingleCategoryGiftCard& operator = (const SingleCategoryGiftCard& other);

    SingleCategoryGiftCard(SingleCategoryGiftCard&& other) noexcept;

    SingleCategoryGiftCard& operator = (SingleCategoryGiftCard&& other) noexcept;

    ~SingleCategoryGiftCard() = default;

    bool canApplyToCategory(int categoryId) const override;
    double calculateDiscount(double originalPrice, int categoryId) const override;
    const char* getType() const override;
    void displayInfo() const override;

    void saveToFile(std::ofstream& file) const override;
    bool loadFromFile(std::ifstream& file) override;

    int getCategoryId() const
    {
        return discountValue;
    }
    double getDiscountPercentage() const
    {
        return discountPercentage;
    }
};

class MultipleCategoryGiftCard : public GiftCard<int*>
{
private:
    int categoryCount;
    int categoryCapacity;
    double discountPercentage;

    void resizeCategoryArray();
    void copyCategoryArray(const MultipleCategoryGiftCard& other);
    void clearCategoryArray();

public:
    MultipleCategoryGiftCard();
    MultipleCategoryGiftCard(const int* categoryIds, int count, double percentage);

    MultipleCategoryGiftCard(const MultipleCategoryGiftCard& other);

    MultipleCategoryGiftCard& operator = (const MultipleCategoryGiftCard& other);

    MultipleCategoryGiftCard(MultipleCategoryGiftCard&& other) noexcept;

    MultipleCategoryGiftCard& operator = (MultipleCategoryGiftCard&& other) noexcept;

    ~MultipleCategoryGiftCard();

    bool canApplyToCategory(int categoryId) const override;
    double calculateDiscount(double originalPrice, int categoryId) const override;
    const char* getType() const override;
    void displayInfo() const override;

    void saveToFile(std::ofstream& file) const override;
    bool loadFromFile(std::ifstream& file) override;

    void addCategory(int categoryId);
    bool removeCategory(int categoryId);

    // Getters
    int getCategoryCount() const
    {
        return categoryCount;
    }
    const int* getCategoryIds() const
    {
        return discountValue;
    }
    double getDiscountPercentage() const
    {
        return discountPercentage;
    }
};

class AllProductsGiftCard : public GiftCard<double>
{
public:
    AllProductsGiftCard();
    AllProductsGiftCard(double percentage);

    AllProductsGiftCard(const AllProductsGiftCard& other);

    AllProductsGiftCard& operator = (const AllProductsGiftCard& other);

    AllProductsGiftCard(AllProductsGiftCard&& other) noexcept;

    AllProductsGiftCard& operator = (AllProductsGiftCard&& other) noexcept;

    ~AllProductsGiftCard() = default;

    bool canApplyToCategory(int categoryId) const override;
    double calculateDiscount(double originalPrice, int categoryId) const override;
    const char* getType() const override;
    void displayInfo() const override;

    void saveToFile(std::ofstream& file) const override;
    bool loadFromFile(std::ifstream& file) override;

    double getDiscountPercentage() const { return discountValue; }
};

class GiftCardFactory
{
public:
    static SingleCategoryGiftCard* createSingleCategory(int category_id, double percentage);

    static MultipleCategoryGiftCard* createMultipleCategory(const int* category_ids, int count, double percentage);

    static AllProductsGiftCard* createAllProducts(double percentage);

    static GiftCard<int>* loadFromString(const char* line);
};