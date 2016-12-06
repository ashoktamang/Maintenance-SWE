//
//  main.cpp
//  Maintenance SWE
//
//  Created by Ashok Tamang on 12/5/16.
//  Copyright © 2016 Howard University. All rights reserved.
//

#include <iostream>
#include <unordered_map>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

unordered_map<char, vector<char>> get_module_tree(string file_name);
void get_transaction_for_explosion(string file_name, char* defects);
vector<char> get_all_transactions(unordered_map<char, vector<char>> module_tree);
vector<char> get_all_unique_modules(unordered_map<char, vector<char>> module_tree);
vector<vector<char>> get_all_paths(unordered_map<char, vector<char>>, char transaction, char defective);

int main(int argc, const char * argv[]) {
    
    // Hashmap, key<string> = name of the module, value <string[]> = its depended modules' name
    
    std::unordered_map<char, std::vector<char>> modules;
    
    std::vector<char> adjacency_lists1;
    adjacency_lists1.push_back('A');
    adjacency_lists1.push_back('B');
    adjacency_lists1.push_back('C');
//    modules['D'] = adjacency_lists1;
    modules['D'].push_back('E');
    modules['D'].push_back('G');
    
    // For iterating the hashmap
    
//    for(std::unordered_map<char, std::vector<char>>::const_iterator iter = modules.begin() ; iter != modules.end() ; iter++)
//    {
//        std::cout << iter->first << ":";
//        for (std::vector<char>::const_iterator jter = iter->second.begin() ; jter != iter->second.end() ; jter++) {
//            std::cout << *jter << " ";
//        }
//        std::cout << std::endl;
//    }
    
    std::unordered_map<char, std::vector<char>> modules1;
    modules1 = get_module_tree("inp.txt");
    for(std::unordered_map<char, std::vector<char>>::const_iterator iter = modules1.begin() ; iter != modules1.end() ; iter++)
    {
        std::cout << iter->first << ":";
        for (std::vector<char>::const_iterator jter = iter->second.begin() ; jter != iter->second.end() ; jter++) {
            std::cout << *jter << " ";
        }
        std::cout << std::endl;
    }
    char defects[2];
    get_transaction_for_explosion("inp.txt", defects);
    cout << "Defected Module: " << defects[0] << endl;
    cout << "Transaction for the explosion: " << defects[1] << endl;
    
    cout << "Transactions: \n";
    vector<char> transactions;
    transactions = get_all_transactions(modules1);
    for (vector<char>::iterator it = transactions.begin() ; it != transactions.end(); ++it)
        cout << *it << ' ';
    
    cout << "\n---------------------------\n";
    
    cout << "The number of transactions: " << transactions.size() << endl;
    
    cout << "\n---------------------------\n";
    
    cout << "Unique Modules:\n";
    vector<char> unique_modules;
    unique_modules = get_all_unique_modules(modules1);
    for (vector<char>::iterator it = unique_modules.begin() ; it != unique_modules.end(); ++it)
        cout << *it << ' ';
    
    cout << "\n---------------------------\n";
    
    cout << "The number of unique modules: " << unique_modules.size() << endl;
    
    return 0;
}

unordered_map<char, vector<char>> get_module_tree(string file_name) {
    // Hashmap, key<string> = name of the module, value <string[]> = its depended modules' name
    
    unordered_map<char, vector<char>> modules;
    vector<char> adjacency_list;
    
    ifstream file;
    // left_module and right_module are separated by a space in the file. and right_module depends on left_module
    char left_module, right_module;
    file.open(file_name);
    while (file) {
        file >> left_module >> right_module;
//        cout << left_module << endl;
//        cout << right_module << endl;
        if (left_module != '*')
            modules[left_module].push_back(right_module);
        else
            break;
    }
    return modules;
}

//Module after the * * is the name of the defective module.
//Module after that defective module is the Transaction for the explosion.
 void get_transaction_for_explosion(string file_name, char* defects) {
     ifstream file;
     // left_module and right_module are separated by a space in the file. and right_module depends on left_module
     char defective_module, transaction;
     char left_module = '\0', right_module;
     file.open(file_name);
     while (file) {
         if (left_module == '*'){
             file >> defective_module >> transaction;
             defects[0] = defective_module;
             defects[1] = transaction;
             break;
         } else
             file >> left_module >> right_module;
     }

}

vector<char> get_all_transactions(unordered_map<char, vector<char>> module_tree) {
    vector<char> transactions;
    vector<char> keys;
    keys.reserve(module_tree.size());
    vector<char> vals;
    vals.reserve(module_tree.size());
    
    for(auto kv : module_tree) {
        keys.push_back(kv.first);
        vals.insert(vals.end(), kv.second.begin(), kv.second.end());
    }
    for (vector<char>::iterator it = keys.begin() ; it != keys.end(); ++it){
        if(find(vals.begin(), vals.end(), *it) == vals.end())
            /* v contains x */
            transactions.push_back(*it);
    }
    return transactions;
}

vector<char> get_all_unique_modules(unordered_map<char, vector<char>> module_tree) {
    vector<char> unique_modules;
    // Only Values in the map can be modules.
    vector<char> vals;
    vals.reserve(module_tree.size());
    
    for(auto kv : module_tree) {
        vals.insert(vals.end(), kv.second.begin(), kv.second.end());
    }
    for (vector<char>::iterator it = vals.begin() ; it != vals.end(); ++it){
        if (!unique_modules.empty()) {
            if(find(unique_modules.begin(), unique_modules.end(), *it) == unique_modules.end())
                unique_modules.push_back(*it);
        } else {
            unique_modules.push_back(*it);
        }
    }
    return unique_modules;
}

vector<vector<char>> get_all_paths(unordered_map<char, vector<char>>, char transaction, char defective) {
    vector<vector<char>> paths;
    
    return paths;
}



