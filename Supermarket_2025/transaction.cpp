#include "transaction.h"

TransactionItem::TransactionItem()
{
    productId = 0;
    strCopy(productName, "", 100);
    quantity = 0.0;
    unitPrice = 0.0;
    totalPrice = 0.0;
}

TransactionItem::TransactionItem(int prodId, const char* prodName, double qty, double price)
{
    productId = prodId;
    strCopy(productName, prodName, 100);
    quantity = qty;
    unitPrice = price;
    totalPrice = qty * price;
}

void TransactionItem::displayInfo() const
{
    std::cout << productName << std::endl;
    std::cout << quantity << "*" << unitPrice << " - " << totalPrice << std::endl;
    std::cout << "###" << std::endl;
}

void TransactionItem::saveToFile(std::ofstream& file) const
{
    file << "ITEM:" << productId << ":" << productName << ":" << quantity << ":" << unitPrice << ":" << totalPrice << std::endl;
}

void TransactionItem::loadFromFile(std::ifstream& file)
{
    char line[200];
    file.getline(line, 200);

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
    strCopy(productName, nameStart, 100);
    token++;

    quantity = 0.0;
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
    if (*token == ':') token++;

    unitPrice = 0.0;
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
                unitPrice = unitPrice * 10 + (*token - '0');
            }
            else 
            {
                unitPrice += (*token - '0') * multiplier;
                multiplier *= 0.1;
            }
        }
        token++;
    }
    if (*token == ':') token++;

    totalPrice = 0.0;
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
                totalPrice = totalPrice * 10 + (*token - '0');
            }
            else
            {
                totalPrice += (*token - '0') * multiplier;
                multiplier *= 0.1;
            }
        }
        token++;
    }
}

void TransactionItem::loadFromString(const char* line)
{
    char workLine[200];
    strCopy(workLine, line, 200);
    char* token = workLine;

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
    strCopy(productName, nameStart, 100);
    token++;

    quantity = 0.0;
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
    if (*token == ':') token++;

    unitPrice = 0.0;
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
                unitPrice = unitPrice * 10 + (*token - '0');
            }
            else
            {
                unitPrice += (*token - '0') * multiplier;
                multiplier *= 0.1;
            }
        }
        token++;
    }
    if (*token == ':') token++;

    totalPrice = 0.0;
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
                totalPrice = totalPrice * 10 + (*token - '0');
            }
            else
            {
                totalPrice += (*token - '0') * multiplier;
                multiplier *= 0.1;
            }
        }
        token++;
    }
}

Transaction::Transaction()
{
    transactionId = 0;
    cashierId = 0;
    strCopy(date, "", DATE_SIZE);
    strCopy(time, "", TIME_SIZE);
    items = nullptr;
    itemCount = 0;
    itemCapacity = 0;
    subtotal = 0.0;
    discountAmount = 0.0;
    finalTotal = 0.0;
    strCopy(voucherCode, "", 6);
    voucherApplied = false;
    expectedItemCount = 0;
}

Transaction::Transaction(int transId, int cashId)
{
    transactionId = transId;
    cashierId = cashId;
    getCurrentDate(date, DATE_SIZE);
    getCurrentTime(time, TIME_SIZE);
    items = nullptr;
    itemCount = 0;
    itemCapacity = 0;
    subtotal = 0.0;
    discountAmount = 0.0;
    finalTotal = 0.0;
    strCopy(voucherCode, "", 6);
    voucherApplied = false;
    expectedItemCount = 0;
}

Transaction::~Transaction()
{
    clearItems();
}

Transaction::Transaction(const Transaction& other)
{
    transactionId = other.transactionId;
    cashierId = other.cashierId;
    strCopy(date, other.date, DATE_SIZE);
    strCopy(time, other.time, TIME_SIZE);
    subtotal = other.subtotal;
    discountAmount = other.discountAmount;
    finalTotal = other.finalTotal;
    strCopy(voucherCode, other.voucherCode, 6);
    voucherApplied = other.voucherApplied;

    items = nullptr;
    itemCount = 0;
    itemCapacity = 0;
    copyItems(other);
}

Transaction& Transaction::operator=(const Transaction& other)
{
    if (this != &other)
    {
        clearItems();

        transactionId = other.transactionId;
        cashierId = other.cashierId;
        strCopy(date, other.date, DATE_SIZE);
        strCopy(time, other.time, TIME_SIZE);
        subtotal = other.subtotal;
        discountAmount = other.discountAmount;
        finalTotal = other.finalTotal;
        strCopy(voucherCode, other.voucherCode, 6);
        voucherApplied = other.voucherApplied;

        items = nullptr;
        itemCount = 0;
        itemCapacity = 0;
        copyItems(other);
    }
    return *this;
}

Transaction::Transaction(Transaction&& other) noexcept
{
    transactionId = other.transactionId;
    cashierId = other.cashierId;
    strCopy(date, other.date, DATE_SIZE);
    strCopy(time, other.time, TIME_SIZE);
    subtotal = other.subtotal;
    discountAmount = other.discountAmount;
    finalTotal = other.finalTotal;
    strCopy(voucherCode, other.voucherCode, 6);
    voucherApplied = other.voucherApplied;

    items = other.items;
    itemCount = other.itemCount;
    itemCapacity = other.itemCapacity;

    other.items = nullptr;
    other.itemCount = 0;
    other.itemCapacity = 0;
}

Transaction& Transaction::operator=(Transaction&& other) noexcept
{
    if (this != &other)
    {
        clearItems();

        transactionId = other.transactionId;
        cashierId = other.cashierId;
        strCopy(date, other.date, DATE_SIZE);
        strCopy(time, other.time, TIME_SIZE);
        subtotal = other.subtotal;
        discountAmount = other.discountAmount;
        finalTotal = other.finalTotal;
        strCopy(voucherCode, other.voucherCode, 6);
        voucherApplied = other.voucherApplied;

        items = other.items;
        itemCount = other.itemCount;
        itemCapacity = other.itemCapacity;

        other.items = nullptr;
        other.itemCount = 0;
        other.itemCapacity = 0;
    }
    return *this;
}

bool Transaction::addItem(int productId, const char* productName, double quantity, double unitPrice)
{
    if (itemCount >= itemCapacity)
    {
        resizeItemArray();
    }

    TransactionItem newItem(productId, productName, quantity, unitPrice);
    items[itemCount] = newItem;
    itemCount++;

    calculateSubtotal();
    return true;
}

void Transaction::calculateSubtotal()
{
    subtotal = 0.0;
    for (int i = 0; i < itemCount; i++)
    {
        subtotal += items[i].getTotalPrice();
    }

    if (!voucherApplied) {
        finalTotal = subtotal;
    }
}

bool Transaction::applyVoucher(const char* voucherCode, double discAmount)
{
    if (!voucherCode || discAmount < 0) return false;

    char tempVoucherCode[6];
    strCopy(tempVoucherCode, voucherCode, 6);
    strCopy(this->voucherCode, tempVoucherCode, 6);
    this->discountAmount = discAmount;
    voucherApplied = true;

    finalTotal = subtotal - discountAmount;
    if (finalTotal < 0)
    {
        finalTotal = 0.0;
    }

    return true;
}

void Transaction::finalizeTransaction()
{
    if (!voucherApplied)
    {
        finalTotal = subtotal;
    }
}

void Transaction::displayReceipt() const
{
    std::cout << "RECEIPT" << std::endl;
    std::cout << "transactionId:" << transactionId << std::endl;
    std::cout << "cashierId:" << cashierId << std::endl;
    std::cout << date << std::endl;
    std::cout << "--------------------" << std::endl;

    for (int i = 0; i < itemCount; i++)
    {
        items[i].displayInfo();
    }

    if (voucherApplied && discountAmount > 0)
    {
        std::cout << "VOUCHER: -" << discountAmount << " (" << voucherCode << ")" << std::endl;
    }

    std::cout << "TOTAL:" << finalTotal << std::endl;
}

void Transaction::saveReceiptToFile() const
{
    char filename[50];
    generateReceiptFilename(filename, 50);

    std::ofstream file(filename);
    if (!file.is_open()) return;

    file << "RECEIPT" << std::endl;
    file << "TRANSACTION_ID:" << transactionId << std::endl;
    file << "CASHIER_ID:" << cashierId << std::endl;
    file << date << std::endl;
    file << "--------------------" << std::endl;

    for (int i = 0; i < itemCount; i++)
    {
        file << items[i].getProductName() << std::endl;
        file << items[i].getQuantity() << "*" << items[i].getUnitPrice() << " - " << items[i].getTotalPrice() << std::endl;
        file << "###" << std::endl;
    }

    if (voucherApplied && discountAmount > 0)
    {
        file << "VOUCHER: -" << discountAmount << " (" << voucherCode << ")" << std::endl;
    }

    file << "TOTAL:" << finalTotal << std::endl;
    file.close();
}

void Transaction::saveToFile(std::ofstream& file) const
{
    file << "TRANSACTION:" << transactionId << ":" << cashierId << ":" << date << ":" << time << ":"
        << subtotal << ":" << discountAmount << ":" << finalTotal << ":" << voucherCode << ":"
        << voucherApplied << ":" << itemCount << std::endl;

    for (int i = 0; i < itemCount; i++)
    {
        items[i].saveToFile(file);
    }
}

void Transaction::loadFromFile(std::ifstream& file)
{
    char line[400];
    file.getline(line, 400);
    loadFromString(line);

    for (int i = 0; i < expectedItemCount; i++)
    {
        if (itemCount >= itemCapacity)
        {
            resizeItemArray();
        }

        TransactionItem item;
        item.loadFromFile(file);

        items[itemCount] = TransactionItem(item.getProductId(), item.getProductName(),
            item.getQuantity(), item.getUnitPrice());
        itemCount++;
    }
}

void Transaction::loadFromString(const char* line)
{
    char workLine[400];
    strCopy(workLine, line, 400);
    char* token = workLine;

    while (*token != ':' && *token != '\0') token++;
    if (*token == ':') token++;

    transactionId = 0;
    while (isDigitChar(*token))
    {
        transactionId = transactionId * 10 + (*token - '0');
        token++;
    }
    if (*token == ':') token++;

    cashierId = 0;
    while (isDigitChar(*token))
    {
        cashierId = cashierId * 10 + (*token - '0');
        token++;
    }
    if (*token == ':') token++;

    char* dateStart = token;
    while (*token != ':' && *token != '\0') token++;
    *token = '\0';
    strCopy(date, dateStart, DATE_SIZE);
    token++;

    char* timeStart = token;
    while (*token != ':' && *token != '\0') token++;
    *token = '\0';
    strCopy(time, timeStart, TIME_SIZE);
    token++;

    subtotal = 0.0;
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
                subtotal = subtotal * 10 + (*token - '0');
            }
            else 
            {
                subtotal += (*token - '0') * multiplier;
                multiplier *= 0.1;
            }
        }
        token++;
    }
    if (*token == ':') token++;

    discountAmount = 0.0;
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
                discountAmount = discountAmount * 10 + (*token - '0');
            }
            else 
            {
                discountAmount += (*token - '0') * multiplier;
                multiplier *= 0.1;
            }
        }
        token++;
    }
    if (*token == ':') token++;

    finalTotal = 0.0;
    multiplier = 1.0;
    foundDot = false;
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
                finalTotal = finalTotal * 10 + (*token - '0');
            }
            else 
            {
                finalTotal += (*token - '0') * multiplier;
                multiplier *= 0.1;
            }
        }
        token++;
    }
    if (*token == ':') token++;

    char* voucherStart = token;
    while (*token != ':' && *token != '\0') token++;
    *token = '\0';
    strCopy(voucherCode, voucherStart, 6);
    token++;

    voucherApplied = (*token == '1');
    token++;
    if (*token == ':') token++;

    expectedItemCount = 0;
    while (isDigitChar(*token)) {
        expectedItemCount = expectedItemCount * 10 + (*token - '0');
        token++;
    }

    clearItems();
}


int Transaction::generateNewTransactionId()
{
    static int nextTransactionId = 1;
    return nextTransactionId++;
}

void Transaction::resizeItemArray()
{
    int newCapacity = (itemCapacity == 0) ? 2 : itemCapacity * 2;
    TransactionItem* newArray = new TransactionItem[newCapacity];

    for (int i = 0; i < itemCount; i++)
    {
        newArray[i] = items[i];
    }

    delete[] items;
    items = newArray;
    itemCapacity = newCapacity;
}

void Transaction::copyItems(const Transaction& other)
{
    if (other.itemCount > 0)
    {
        itemCapacity = other.itemCapacity;
        items = new TransactionItem[itemCapacity];

        for (int i = 0; i < other.itemCount; i++)
        {
            items[i] = other.items[i];
        }
        itemCount = other.itemCount;
    }
}

void Transaction::clearItems()
{
    delete[] items;
    items = nullptr;
    itemCount = 0;
    itemCapacity = 0;
}

void Transaction::generateReceiptFilename(char* filename, int maxSize) const
{
    // Format receipt_00001.txt
    strCopy(filename, "receipt_", maxSize);

    char idStr[10];
    intToString(transactionId, idStr, 10);

    if (transactionId < 10)
    {
        strCopy(filename + 8, "0000", maxSize - 8);
        strCopy(filename + 12, idStr, maxSize - 12);
        strCopy(filename + 13, ".txt", maxSize - 13);
    }
    else if (transactionId < 100)
    {
        strCopy(filename + 8, "000", maxSize - 8);
        strCopy(filename + 11, idStr, maxSize - 11);
        strCopy(filename + 13, ".txt", maxSize - 13);
    }
    else if (transactionId < 1000)
    {
        strCopy(filename + 8, "00", maxSize - 8);
        strCopy(filename + 10, idStr, maxSize - 10);
        strCopy(filename + 13, ".txt", maxSize - 13);
    }
    else 
    {
        strCopy(filename + 8, idStr, maxSize - 8);
        strCopy(filename + 8 + getStrLength(idStr), ".txt", maxSize - 8 - getStrLength(idStr));
    }
}

TransactionManager::TransactionManager()
{
    transactions = nullptr;
    transactionCount = 0;
    transactionCapacity = 0;
    productManager = nullptr;
}

TransactionManager::TransactionManager(ProductManager* prodMgr)
{
    transactions = nullptr;
    transactionCount = 0;
    transactionCapacity = 0;
    productManager = prodMgr;
}

TransactionManager::~TransactionManager()
{
    clearTransactions();
}

Transaction* TransactionManager::startNewTransaction(int cashierId)
{
    int newId = Transaction::generateNewTransactionId();
    Transaction* transaction = new Transaction(newId, cashierId);
    return transaction;
}

bool TransactionManager::processSellCommand(int cashierId)
{
    if (!productManager)
    {
        std::cout << "Error: Product manager not available." << std::endl;
        return false;
    }

    Transaction* transaction = startNewTransaction(cashierId);

    std::cout << "Products:" << std::endl;
    displayAvailableProducts();
    std::cout << "Transaction ID: " << transaction->getTransactionId() << std::endl;
    std::cout << "Price: 0.00" << std::endl;

    while (true)
    {
        std::cout << "Enter product ID to sell. Enter END to end the transaction:" << std::endl;
        std::cout << "> ";

        int productId = getUserProductChoice();
        if (productId == -1) 
        {
            break;
        }

        Product* product = productManager->findProductById(productId);
        if (!product)
        {
            std::cout << "Product not found. Please try again." << std::endl;
            continue;
        }

        double quantity = getUserQuantityChoice(product);
        if (quantity <= 0)
        {
            std::cout << "Invalid quantity. Please try again." << std::endl;
            continue;
        }

        if (!product->hasEnoughQuantity(quantity))
        {
            std::cout << "Insufficient stock. Available: " << product->getAvailableQuantity() << std::endl;
            continue;
        }

        addProductToCurrentTransaction(transaction, productId, quantity);

        productManager->updateProductStock(productId, quantity, false);

        std::cout << "---------" << std::endl;
        std::cout << "Products:" << std::endl;
        displayAvailableProducts();
        std::cout << "Transaction ID: " << transaction->getTransactionId() << std::endl;
        std::cout << "Price: " << transaction->getSubtotal() << std::endl;
    }

    char voucherCode[6];
    bool wantsVoucher = getUserVoucherChoice(voucherCode, 6);

    if (wantsVoucher) {
        if (validateAndApplyVoucher(transaction, voucherCode)) 
        {
            double discountPercent = (transaction->getDiscountAmount() / transaction->getSubtotal()) * 100;
            std::cout << (int)discountPercent << "% applied! Transaction complete!" << std::endl;
        }
        else 
        {
            std::cout << "Invalid voucher code." << std::endl;
        }
    }

    completeTransaction(transaction);

    char receiptFilename[50];
    transaction->generateReceiptFilename(receiptFilename, 50);
    std::cout << "Receipt saved as: " << receiptFilename << std::endl;
    std::cout << "Total: " << transaction->getFinalTotal() << " lv." << std::endl;

    return true;
}

bool TransactionManager::addProductToCurrentTransaction(Transaction* transaction, int productId, double quantity)
{
    if (!transaction || !productManager) return false;

    Product* product = productManager->findProductById(productId);
    if (!product) return false;

    return transaction->addItem(productId, product->getName(), quantity, product->getPrice());
}

bool TransactionManager::applyVoucherToTransaction(Transaction* transaction, const char* voucherCode)
{
    if (!transaction || !voucherCode) return false;

    double discountAmount = calculateVoucherDiscount(transaction, voucherCode);
    if (discountAmount > 0)
    {
        return transaction->applyVoucher(voucherCode, discountAmount);
    }

    return false;
}

void TransactionManager::completeTransaction(Transaction* transaction)
{
    if (!transaction) return;

    transaction->finalizeTransaction();
    transaction->saveReceiptToFile();
    addTransaction(transaction);
}

bool TransactionManager::addTransaction(Transaction* transaction)
{
    if (!transaction) return false;

    if (transactionCount >= transactionCapacity) {
        resizeTransactionArray();
    }

    transactions[transactionCount] = transaction;
    transactionCount++;
    return true;
}

Transaction* TransactionManager::findTransactionById(int id)
{
    for (int i = 0; i < transactionCount; i++) 
    {
        if (transactions[i] && transactions[i]->getTransactionId() == id) {
            return transactions[i];
        }
    }
    return nullptr;
}

void TransactionManager::listAllTransactions() const
{
    std::cout << "=== ALL TRANSACTIONS ===" << std::endl;
    for (int i = 0; i < transactionCount; i++)
    {
        if (transactions[i]) {
            std::cout << "Transaction ID: " << transactions[i]->getTransactionId()
                << ", Cashier: " << transactions[i]->getCashierId()
                << ", Date: " << transactions[i]->getDate()
                << ", Total: " << transactions[i]->getFinalTotal() << " lv." << std::endl;
        }
    }
}

void TransactionManager::listTransactionsByCashier(int cashierId) const
{
    std::cout << "=== TRANSACTIONS BY CASHIER " << cashierId << " ===" << std::endl;
    bool found = false;

    for (int i = 0; i < transactionCount; i++) 
    {
        if (transactions[i] && transactions[i]->getCashierId() == cashierId)
        {
            std::cout << "Transaction ID: " << transactions[i]->getTransactionId()
                << ", Date: " << transactions[i]->getDate()
                << ", Total: " << transactions[i]->getFinalTotal() << " lv." << std::endl;
            found = true;
        }
    }

    if (!found)
    {
        std::cout << "No transactions found for this cashier." << std::endl;
    }
}

void TransactionManager::saveAllTransactions(const char* filename) const
{
    std::ofstream file(filename);
    if (!file.is_open()) return;

    for (int i = 0; i < transactionCount; i++)
    {
        if (transactions[i])
        {
            transactions[i]->saveToFile(file);
        }
    }

    file.close();
}

void TransactionManager::loadAllTransactions(const char* filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) return;

    char line[400];
    while (file.getline(line, 400))
    {
        if (line[0] != '\0' && line[0] == 'T') 
        {
            Transaction* transaction = new Transaction();

            transaction->loadFromString(line);

            for (int i = 0; i < transaction->getExpectedItemCount(); i++)
            {
                char itemLine[200];
                if (file.getline(itemLine, 200) && itemLine[0] == 'I') 
                { // ITEM
                    TransactionItem item;
                    item.loadFromString(itemLine);

                    // Add item to transaction using existing method
                    transaction->addItem(item.getProductId(), item.getProductName(),
                        item.getQuantity(), item.getUnitPrice());
                }
            }

            addTransaction(transaction);
        }
    }

    file.close();
}

void TransactionManager::resizeTransactionArray()
{
    int newCapacity = (transactionCapacity == 0) ? 2 : transactionCapacity * 2;
    Transaction** newArray = new Transaction * [newCapacity];

    for (int i = 0; i < transactionCount; i++)
    {
        newArray[i] = transactions[i];
    }

    delete[] transactions;
    transactions = newArray;
    transactionCapacity = newCapacity;
}

void TransactionManager::clearTransactions()
{
    for (int i = 0; i < transactionCount; i++)
    {
        delete transactions[i];
    }
    delete[] transactions;
    transactions = nullptr;
    transactionCount = 0;
    transactionCapacity = 0;
}

void TransactionManager::displayAvailableProducts() const
{
    if (productManager)
    {
        productManager->listAllProducts();
    }
}

int TransactionManager::getUserProductChoice() const
{
    char input[20];
    std::cin.getline(input, 20);

    if (strCompare(input, "END") || strCompare(input, "end"))
    {
        return -1;
    }

    if (isValidNumber(input))
    {
        int productId = 0;
        for (int i = 0; input[i] != '\0'; i++) 
        {
            if (isDigitChar(input[i]))
            {
                productId = productId * 10 + (input[i] - '0');
            }
        }
        return productId;
    }

    return 0;
}

double TransactionManager::getUserQuantityChoice(Product* product) const
{
    std::cout << "Enter quantity:" << std::endl;
    std::cout << "> ";

    char input[20];
    std::cin.getline(input, 20);

    if (isValidFloat(input))
    {
        double quantity = 0.0;
        double multiplier = 1.0;
        bool foundDot = false;

        for (int i = 0; input[i] != '\0'; i++)
        {
            if (input[i] == '.')
            {
                foundDot = true;
                multiplier = 0.1;
            }
            else if (isDigitChar(input[i]))
            {
                if (!foundDot)
                {
                    quantity = quantity * 10 + (input[i] - '0');
                }
                else
                {
                    quantity += (input[i] - '0') * multiplier;
                    multiplier *= 0.1;
                }
            }
        }
        return quantity;
    }

    return 0.0;
}

bool TransactionManager::getUserVoucherChoice(char* voucherCode, int maxSize) const
{
    std::cout << "Add voucher: (Y/N)? ";

    char input[10];
    std::cin.getline(input, 10);

    if (strCompare(input, "Y") || strCompare(input, "y") || strCompare(input, "yes") || strCompare(input, "YES"))
    {
        std::cout << "Enter voucher: ";
        std::cin.getline(voucherCode, maxSize);
        return true;
    }

    return false;
}

bool TransactionManager::validateAndApplyVoucher(Transaction* transaction, const char* voucherCode)
{
    if (getStrLength(voucherCode) == 5) 
    {
        double discountAmount = calculateVoucherDiscount(transaction, voucherCode);
        if (discountAmount > 0) 
        {
            return transaction->applyVoucher(voucherCode, discountAmount);
        }
    }
    return false;
}

double TransactionManager::calculateVoucherDiscount(Transaction* transaction, const char* voucherCode)
{
    if (!transaction || !voucherCode) return 0.0;

    if (getStrLength(voucherCode) != 5) return 0.0;

    if (!isDigitChar(voucherCode[0]) ||
        !(voucherCode[1] >= 'A' && voucherCode[1] <= 'Z') ||
        !isDigitChar(voucherCode[2]) ||
        !(voucherCode[3] >= 'A' && voucherCode[3] <= 'Z') ||
        !isDigitChar(voucherCode[4]))
    {
        return 0.0;
    }

    double subtotal = transaction->getSubtotal();

    char firstChar = voucherCode[0];
    double discountPercentage = 0.0;

    switch (firstChar)
    {
    case '1': discountPercentage = 0.05; break;
    case '2': discountPercentage = 0.10; break;  
    case '3': discountPercentage = 0.15; break;
    case '4': discountPercentage = 0.20; break;
    case '5': discountPercentage = 0.25; break;
    default:  discountPercentage = 0.10; break;
    }

    double discountAmount = subtotal * discountPercentage;

    if (discountAmount > subtotal)
    {
        discountAmount = subtotal;
    }

    return discountAmount;
}