#pragma once

#include "utils.h"
#include "product.h"
#include "voucher.h"
#include <iostream>
#include <fstream>

class TransactionItem
{
private:
    int productId;
    char productName[100];
    double quantity;
    double unitPrice;
    double totalPrice;

public:
    TransactionItem();
    TransactionItem(int prodId, const char* prodName, double qty, double unitPrice);

    int getProductId() const
    {
        return productId;
    }
    const char* getProductName() const
    {
        return productName;
    }
    double getQuantity() const
    {
        return quantity;
    }
    double getUnitPrice() const
    {
        return unitPrice;
    }
    double getTotalPrice() const
    {
        return totalPrice;
    }

    void displayInfo() const;
    void saveToFile(std::ofstream& file) const;
    void loadFromFile(std::ifstream& file);
    void loadFromString(const char* line);
};

class Transaction
{
private:
    int transactionId;
    int cashierId;
    char date[DATE_SIZE];
    char time[TIME_SIZE];
    TransactionItem* items;
    int itemCount;
    int itemCapacity;
    double subtotal;
    double discountAmount;
    double finalTotal;
    char voucherCode[6];
    bool voucherApplied;
    int expectedItemCount;

public:
    Transaction();
    Transaction(int transactionId, int cashierId);

    ~Transaction();

    Transaction(const Transaction& other);
    Transaction& operator=(const Transaction& other);
    Transaction(Transaction&& other) noexcept;
    Transaction& operator=(Transaction&& other) noexcept;

    bool addItem(int productId, const char* productName, double quantity, double unitPrice);
    void calculateSubtotal();
    bool applyVoucher(const char* voucherCode, double discountAmount);
    void finalizeTransaction();

    void displayReceipt() const;
    void saveReceiptToFile() const;
    void saveToFile(std::ofstream& file) const;
    void loadFromFile(std::ifstream& file);
    void loadFromString(const char* line);

    int getTransactionId() const
    {
        return transactionId;
    }
    int getCashierId() const
    {
        return cashierId;
    }
    const char* getDate() const
    {
        return date;
    }
    const char* getTime() const
    {
        return time;
    }
    int getItemCount() const
    {
        return itemCount;
    }
    double getSubtotal() const
    {
        return subtotal;
    }
    double getDiscountAmount() const
    {
        return discountAmount;
    }
    double getFinalTotal() const
    {
        return finalTotal;
    }
    int getExpectedItemCount() const
    {
        return expectedItemCount;
    }
    const char* getVoucherCode() const
    {
        return voucherCode;
    }
    bool isVoucherApplied() const
    {
        return voucherApplied;
    }
    void generateReceiptFilename(char* filename, int maxSize) const;

    static int generateNewTransactionId();

private:
    void resizeItemArray();
    void copyItems(const Transaction& other);
    void clearItems();
};

class TransactionManager
{
private:
    Transaction** transactions;
    int transactionCount;
    int transactionCapacity;
    ProductManager* productManager;

public:
    TransactionManager();
    TransactionManager(ProductManager* prodMgr);
    ~TransactionManager();

    Transaction* startNewTransaction(int cashierId);
    bool processSellCommand(int cashierId);
    bool addProductToCurrentTransaction(Transaction* transaction, int productId, double quantity);
    bool applyVoucherToTransaction(Transaction* transaction, const char* voucherCode);
    void completeTransaction(Transaction* transaction);

    bool addTransaction(Transaction* transaction);
    Transaction* findTransactionById(int id);
    void listAllTransactions() const;
    void listTransactionsByCashier(int cashierId) const;

    void saveAllTransactions(const char* filename) const;
    void loadAllTransactions(const char* filename);

    void setProductManager(ProductManager* prodMgr)
    {
        productManager = prodMgr;
    }

    int getTransactionCount() const
    {
        return transactionCount;
    }

private:
    void resizeTransactionArray();
    void clearTransactions();

    void displayAvailableProducts() const;
    int getUserProductChoice() const;
    double getUserQuantityChoice(Product* product) const;
    bool getUserVoucherChoice(char* voucherCode, int maxSize) const;

    bool validateAndApplyVoucher(Transaction* transaction, const char* voucherCode);
    double calculateVoucherDiscount(Transaction* transaction, const char* voucherCode);
};
