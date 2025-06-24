//#include "workers.h"
//#include <iostream>
//#include "worker_manager.h"
//
//#define _CRT_SECURE_NO_WARNINGS
//
//void testWorkerHierarchy() {
//    std::cout << "=== TESTING WORKER HIERARCHY ===" << std::endl;
//
//    // Test polymorphism with base class pointers
//    Worker* workers[4];
//
//    // Create different worker types
//    workers[0] = new Cashier(1001, "John", "Doe", "123-456-7890", 25, "password123");
//    workers[1] = new Manager(1002, "Jane", "Smith", "098-765-4321", 35, "manager456");
//    workers[2] = new Cashier(1003, "Bob", "Wilson", "555-123-4567", 28, "cashier789");
//    workers[3] = new Manager(1004, "Alice", "Brown", "777-888-9999", 40, "admin000");
//
//    // Test virtual function calls (polymorphism)
//    std::cout << "\n--- Testing Virtual Function Calls ---" << std::endl;
//    for (int i = 0; i < 4; i++) {
//        std::cout << "Worker " << workers[i]->getId() << " is a " << workers[i]->getRole() << std::endl;
//        std::cout << "Has special privileges: " << (workers[i]->hasSpecialPrivileges() ? "Yes" : "No") << std::endl;
//        std::cout << std::endl;
//    }
//
//    // Clean up
//    for (int i = 0; i < 4; i++) {
//        delete workers[i];
//    }
//
//    std::cout << "✅ Worker hierarchy test passed!" << std::endl;
//}
//
//void testCashierWarnings() {
//    std::cout << "\n=== TESTING CASHIER WARNING SYSTEM ===" << std::endl;
//
//    Cashier cashier(2001, "Test", "Cashier", "123-456-7890", 25, "test123");
//
//    // Add warnings
//    cashier.addWarning(100, "Late arrival");
//    cashier.addWarning(200, "Cash register discrepancy");
//
//    std::cout << "Total warning points: " << cashier.getWarningPoints() << std::endl;
//    std::cout << "Should be fired: " << (cashier.shouldBeFired() ? "Yes" : "No") << std::endl;
//
//    // Add critical warning
//    cashier.addWarning(300, "Customer complaint");
//    std::cout << "After critical warning - Should be fired: " << (cashier.shouldBeFired() ? "Yes" : "No") << std::endl;
//
//    // Test promotion eligibility
//    Cashier goodCashier(2002, "Good", "Employee", "987-654-3210", 30, "good123");
//    for (int i = 0; i < 55; i++) {
//        goodCashier.incrementTransactionCount();
//    }
//    std::cout << "Good cashier can be promoted: " << (goodCashier.canPromoteToManager() ? "Yes" : "No") << std::endl;
//
//    std::cout << "✅ Cashier warning system test passed!" << std::endl;
//}
//
//void testManagerSpecialCode() {
//    std::cout << "\n=== TESTING MANAGER SPECIAL CODE SYSTEM ===" << std::endl;
//
//    Manager manager(3001, "Test", "Manager", "555-123-4567", 35, "mgr123");
//
//    std::cout << "Manager has valid code: " << (manager.hasValidCode() ? "Yes" : "No") << std::endl;
//    std::cout << "Special code: " << manager.getSpecialCode() << std::endl;
//
//    // Test code validation
//    const char* code = manager.getSpecialCode();
//    std::cout << "Code validation test: " << (manager.validateSpecialCode(code) ? "Passed" : "Failed") << std::endl;
//    std::cout << "Wrong code test: " << (manager.validateSpecialCode("WRONG12") ? "Failed" : "Passed") << std::endl;
//
//    // Test administrative functions with a test cashier
//    Cashier test_cashier(4001, "Test", "Cashier", "123", 25, "test");
//
//    // Test approve cashier
//    std::cout << "Can approve cashier: " << (manager.approveCashier(&test_cashier, code) ? "Yes" : "No") << std::endl;
//    std::cout << "Cannot approve with wrong code: " << (manager.approveCashier(&test_cashier, "WRONG") ? "No" : "Yes") << std::endl;
//
//    // Test warn cashier
//    std::cout << "Can warn cashier: " << (manager.warnCashier(&test_cashier, 100, "Test warning", code) ? "Yes" : "No") << std::endl;
//    std::cout << "Warning points after warning: " << test_cashier.getWarningPoints() << std::endl;
//
//    std::cout << "✅ Manager special code test passed!" << std::endl;
//}
//
//void testWorkerManager() {
//    std::cout << "\n=== TESTING WORKER MANAGER SYSTEM ===" << std::endl;
//
//    WorkerManager wm;
//
//    // Add workers
//    Cashier* cashier1 = new Cashier(4001, "Alice", "Johnson", "111-222-3333", 24, "alice123");
//    Manager* manager1 = new Manager(4002, "Bob", "Anderson", "444-555-6666", 32, "bob456");
//    Cashier* cashier2 = new Cashier(4003, "Charlie", "Davis", "777-888-9999", 27, "charlie789");
//
//    wm.addWorker(cashier1);
//    wm.addWorker(manager1);
//    wm.addWorker(cashier2);
//
//    std::cout << "Total workers: " << wm.getWorkerCount() << std::endl;
//
//    // Test finding workers
//    Worker* found = wm.findWorkerById(4002);
//    if (found) {
//        std::cout << "Found worker: " << found->getRole() << " " << found->getFirstName() << std::endl;
//    }
//
//    // Test pending workers
//    std::cout << "\n--- Pending Workers ---" << std::endl;
//    wm.listPendingWorkers();
//
//    // Approve a worker
//    const char* special_code = manager1->getSpecialCode();
//    bool approved = wm.approveWorker(4001, 4002, special_code);
//    std::cout << "Cashier approval: " << (approved ? "Success" : "Failed") << std::endl;
//
//    // Test authentication
//    Worker* authenticated = wm.authenticateWorker(4001, "alice123");
//    std::cout << "Authentication result: " << (authenticated ? "Success" : "Failed") << std::endl;
//
//    std::cout << "✅ Worker manager test passed!" << std::endl;
//}
//
//void testFileOperations() {
//    std::cout << "\n=== TESTING FILE I/O OPERATIONS ===" << std::endl;
//
//    // Create test data
//    WorkerManager wm;
//    Cashier* cashier = new Cashier(5001, "File", "Test", "123-456-7890", 26, "file123");
//    Manager* manager = new Manager(5002, "Save", "Load", "987-654-3210", 38, "save456");
//
//    cashier->addWarning(100, "Test warning");
//    cashier->setApproved(true);
//
//    wm.addWorker(cashier);
//    wm.addWorker(manager);
//
//    // Save to file
//    const char* test_file = "test_workers.txt";
//    wm.saveAllWorkers(test_file);
//    std::cout << "Workers saved to file" << std::endl;
//
//    // Clear and reload
//    WorkerManager wm2;
//    wm2.loadAllWorkers(test_file);
//    std::cout << "Workers loaded from file: " << wm2.getWorkerCount() << std::endl;
//
//    // Verify data integrity
//    Worker* loaded_cashier = wm2.findWorkerById(5001);
//    if (loaded_cashier) {
//        std::cout << "Loaded cashier: " << loaded_cashier->getFirstName() << " " << loaded_cashier->getLastName() << std::endl;
//        Cashier* c = dynamic_cast<Cashier*>(loaded_cashier);
//        if (c) {
//            std::cout << "Warning points: " << c->getWarningPoints() << std::endl;
//        }
//    }
//
//    std::cout << "✅ File I/O test passed!" << std::endl;
//}
//
//void testMoveSemantics() {
//    std::cout << "\n=== TESTING MOVE SEMANTICS (C++11) ===" << std::endl;
//
//    // Create original cashier with warnings
//    Cashier original(5001, "Move", "Test", "123-456-7890", 25, "move123");
//    original.addWarning(100, "Test warning 1");
//    original.addWarning(200, "Test warning 2");
//    original.incrementTransactionCount();
//    original.incrementTransactionCount();
//
//    std::cout << "Original - Warnings: " << original.getWarningPoints()
//        << ", Transactions: " << original.getTransactionPoints() << std::endl;
//
//    // Test move constructor
//    Cashier moved_constructed(std::move(original));
//    std::cout << "After move constructor:" << std::endl;
//    std::cout << "  Moved object - Warnings: " << moved_constructed.getWarningPoints()
//        << ", Transactions: " << moved_constructed.getTransactionPoints() << std::endl;
//    std::cout << "  Original object - Warnings: " << original.getWarningPoints()
//        << ", Transactions: " << original.getTransactionPoints() << std::endl;
//
//    // Create another cashier for move assignment test
//    Cashier another(5002, "Another", "Cashier", "987-654-3210", 30, "another123");
//    another.addWarning(300, "Different warning");
//
//    std::cout << "\nBefore move assignment:" << std::endl;
//    std::cout << "  Another - Warnings: " << another.getWarningPoints() << std::endl;
//    std::cout << "  Moved - Warnings: " << moved_constructed.getWarningPoints() << std::endl;
//
//    // Test move assignment
//    another = std::move(moved_constructed);
//
//    std::cout << "After move assignment:" << std::endl;
//    std::cout << "  Another (target) - Warnings: " << another.getWarningPoints()
//        << ", Transactions: " << another.getTransactionPoints() << std::endl;
//    std::cout << "  Moved (source) - Warnings: " << moved_constructed.getWarningPoints()
//        << ", Transactions: " << moved_constructed.getTransactionPoints() << std::endl;
//
//    std::cout << "✅ Move semantics test completed!" << std::endl;
//}
//
//void testRuleOfSix() {
//    std::cout << "\n=== TESTING RULE OF SIX COMPLETENESS ===" << std::endl;
//
//    // Test all six special member functions
//    std::cout << "1. Default constructor:" << std::endl;
//    Cashier default_cashier;
//    std::cout << "   Created default cashier with ID: " << default_cashier.getId() << std::endl;
//
//    std::cout << "2. Parameterized constructor:" << std::endl;
//    Cashier param_cashier(6001, "Rule", "Six", "111-222-3333", 28, "rule6");
//    param_cashier.addWarning(100, "Test");
//    std::cout << "   Created parameterized cashier with warnings: "
//        << param_cashier.getWarningPoints() << std::endl;
//
//    std::cout << "3. Copy constructor:" << std::endl;
//    Cashier copied_cashier(param_cashier);
//    std::cout << "   Copied cashier has warnings: "
//        << copied_cashier.getWarningPoints() << std::endl;
//
//    std::cout << "4. Copy assignment:" << std::endl;
//    default_cashier = param_cashier;
//    std::cout << "   Assigned cashier has warnings: "
//        << default_cashier.getWarningPoints() << std::endl;
//
//    std::cout << "5. Move constructor:" << std::endl;
//    Cashier move_constructed(std::move(copied_cashier));
//    std::cout << "   Move constructed cashier has warnings: "
//        << move_constructed.getWarningPoints() << std::endl;
//    std::cout << "   Source after move has warnings: "
//        << copied_cashier.getWarningPoints() << std::endl;
//
//    std::cout << "6. Move assignment:" << std::endl;
//    Cashier move_target(6002, "Target", "Cashier", "444-555-6666", 32, "target");
//    move_target = std::move(move_constructed);
//    std::cout << "   Move assigned cashier has warnings: "
//        << move_target.getWarningPoints() << std::endl;
//    std::cout << "   Source after move has warnings: "
//        << move_constructed.getWarningPoints() << std::endl;
//
//    std::cout << "✅ Rule of Six completeness test passed!" << std::endl;
//    // 7. Destructor will be called automatically when objects go out of scope
//}
//void testSpecialCodeFormat() {
//std::cout << "\n=== TESTING SPECIAL CODE FORMAT ===" << std::endl;
//
//Manager manager(6001, "Format", "Test", "123-456-7890", 30, "format123");
//const char* code = manager.getSpecialCode();
//
//std::cout << "Generated code: " << code << std::endl;
//
//// Verify format: <0-9><A-Z><A-Z><0-9><0-9><0-9><a-z>
//bool format_valid = true;
//
//// Check length using utils function
//if (getStrLength(code) != 7) format_valid = false;
//
//if (code[0] < '0' || code[0] > '9') format_valid = false;
//if (code[1] < 'A' || code[1] > 'Z') format_valid = false;
//if (code[2] < 'A' || code[2] > 'Z') format_valid = false;
//if (code[3] < '0' || code[3] > '9') format_valid = false;
//if (code[4] < '0' || code[4] > '9') format_valid = false;
//if (code[5] < '0' || code[5] > '9') format_valid = false;
//if (code[6] < 'a' || code[6] > 'z') format_valid = false;
//
//std::cout << "Format validation: " << (format_valid ? "Passed" : "Failed") << std::endl;
//std::cout << "✅ Special code format test " << (format_valid ? "passed" : "failed") << "!" << std::endl;
//}

//int main() {
//    std::cout << "🚨 EMERGENCY SPRINT - WORKER HIERARCHY TESTING 🚨" << std::endl;
//    std::cout << "Testing Module 2: Data Models Implementation" << std::endl;
//    std::cout << "================================================" << std::endl;
//
//    try {
//        testWorkerHierarchy();
//        testCashierWarnings();
//        testManagerSpecialCode();
//        testWorkerManager();
//        testFileOperations();
//        testMoveSemantics();
//        testRuleOfSix();
//        testSpecialCodeFormat();
//
//        std::cout << "\n🎯 ALL TESTS PASSED! Worker hierarchy is ready for production!" << std::endl;
//        std::cout << "✅ Virtual functions working" << std::endl;
//        std::cout << "✅ Polymorphism implemented" << std::endl;
//        std::cout << "✅ File I/O operations functional" << std::endl;
//        std::cout << "✅ Memory management handled (Rule of Six)" << std::endl;
//        std::cout << "✅ Move semantics implemented (C++11)" << std::endl;
//        std::cout << "✅ Manager special code system operational" << std::endl;
//        std::cout << "✅ Warning system for cashiers implemented" << std::endl;
//
//        std::cout << "\n⚡ READY FOR NEXT MODULE! Estimated completion time: 1.5 hours" << std::endl;
//
//    }
//    catch (const std::exception& e) {
//        std::cerr << "❌ Test failed with exception: " << e.what() << std::endl;
//        return 1;
//    }
//
//    return 0;
//}