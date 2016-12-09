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
#include <stack>
#include <algorithm>
#include <stdexcept>

using namespace std;

unordered_map<string, vector<string> > get_module_tree(string file_name);

void get_transaction_for_explosion(string file_name, string* defective_module, string* transaction);

vector<string> get_all_transactions(unordered_map<string, vector<string> > module_tree);

bool is_transaction(string module_name, vector<string> transactions);

vector<string> get_all_unique_modules(unordered_map<string, vector<string> > module_tree);

void get_all_paths(string transaction, string defective, unordered_map<string, vector<string> > module_tree, vector<string> nodes);

void print_all_paths_until(string u, string d, unordered_map<string, bool> visited, string* path, int &path_index, unordered_map<string, vector<string> > module_tree);

void get_explosion(string transaction, unordered_map<string, vector<string> > module_tree, vector<string> nodes);

void print_all_explosion(string transaction, unordered_map<string, bool> visited, string* path, int &path_index, unordered_map<string, vector<string> > module_tree);

int main(int argc, const char * argv[]) {
    
    // Hashmap, key<string> = name of the module, value <string[]> = its depended modules' name
    unordered_map<string, vector<string> > modules1;
    modules1 = get_module_tree("inp1.txt");
    for(unordered_map<string, vector<string> >::const_iterator iter = modules1.begin() ; iter != modules1.end() ; iter++)
    {
        cout << iter->first << ":";
        for (vector<string>::const_iterator jter = iter->second.begin() ; jter != iter->second.end() ; jter++) {
            cout << *jter << " ";
        }
        cout << std::endl;
    }
    string transaction, defective;
    get_transaction_for_explosion("inp1.txt", &defective, &transaction);
    cout << "\nDefected Module: " << defective << endl;
    cout << "Transaction for the explosion: " << transaction << endl;
    
    cout << "Transactions: ";
    vector<string> transactions;
    transactions = get_all_transactions(modules1);
    for (vector<string>::iterator it = transactions.begin() ; it != transactions.end(); ++it)
        cout << *it << ' ';
    
    cout << "\n---------------------------\n";
    
    cout << "The number of transactions: " << transactions.size() << endl;
    
    cout << "\n---------------------------\n";
    
    cout << "Unique Modules:\n";
    vector<string> unique_modules;
    unique_modules = get_all_unique_modules(modules1);
    for (vector<string>::iterator it = unique_modules.begin() ; it != unique_modules.end(); ++it)
        cout << *it << ' ';
    
    cout << "\n---------------------------\n";
    
    cout << "The number of unique modules: " << unique_modules.size() << endl;
    
    // Get all the paths from node u to nove v
    vector<string> nodes;
    nodes = transactions;
    nodes.insert(nodes.end(), unique_modules.begin(), unique_modules.end());
    cout << "All nodes.\n";
    for (vector<string>::iterator it = nodes.begin() ; it != nodes.end(); ++it)
        cout << *it << ' ';
    
    printf("\nGet all the paths from node %s to %s: \n", transaction.c_str(), defective.c_str());
    get_all_paths(transaction, defective, modules1, nodes);
    
    cout << "Explosion from the transaction: " << transaction << endl;
    if (is_transaction(transaction, transactions)) {
        throw invalid_argument("Given transaction is only a module. Please provide the correct argument.");
    }
    get_explosion(transaction, modules1, nodes);
    
    return 0;
}

unordered_map<string, vector<string> > get_module_tree(string file_name) {
    // Hashmap, key<string> = name of the module, value <string[]> = its depended modules' name
    
    unordered_map<string, vector<string> > modules;
    vector<string> adjacency_list;
    
    ifstream file;
    // left_module and right_module are separated by a space in the file. and right_module depends on left_module
    string left_module, right_module;
    file.open(file_name);
    while (file) {
        file >> left_module >> right_module;
//        cout << left_module << " " << right_module << endl;
//        cout << right_module << endl;
        if (left_module != "*")
            modules[left_module].push_back(right_module);
        else
            break;
    }
    return modules;
}

//Module after the * * is the name of the defective module.
//Module after that defective module is the Transaction for the explosion.
 void get_transaction_for_explosion(string file_name, string* defective_module, string* transaction) {
     ifstream file;
     // left_module and right_module are separated by a space in the file. and right_module depends on left_module
     string left_module, right_module;
     file.open(file_name);
     while (file) {
         if (left_module == "*"){
             file >> left_module >> right_module;
             break;
         } else
             file >> left_module >> right_module;
     }
     *defective_module = left_module;
     *transaction = right_module;

}

vector<string> get_all_transactions(unordered_map<string, vector<string> > module_tree) {
    vector<string> transactions;
    vector<string> keys;
    keys.reserve(module_tree.size());
    vector<string> vals;
    vals.reserve(module_tree.size());
    
    for(auto kv : module_tree) {
        keys.push_back(kv.first);
        vals.insert(vals.end(), kv.second.begin(), kv.second.end());
    }
    for (vector<string>::iterator it = keys.begin() ; it != keys.end(); ++it){
        if(find(vals.begin(), vals.end(), *it) == vals.end())
            /* v contains x */
            transactions.push_back(*it);
    }
    return transactions;
}

bool is_transaction(string module_name, vector<string> transactions) {
    return find(transactions.begin(), transactions.end(), module_name) == transactions.end();
}

vector<string> get_all_unique_modules(unordered_map<string, vector<string> > module_tree) {
    vector<string> unique_modules;
    // Only Values in the map can be modules.
    vector<string> vals;
    vals.reserve(module_tree.size());
    
    for(auto kv : module_tree) {
        vals.insert(vals.end(), kv.second.begin(), kv.second.end());
    }
    for (vector<string>::iterator it = vals.begin() ; it != vals.end(); ++it){
        if (!unique_modules.empty()) {
            if(find(unique_modules.begin(), unique_modules.end(), *it) == unique_modules.end())
                unique_modules.push_back(*it);
        } else {
            unique_modules.push_back(*it);
        }
    }
    return unique_modules;
}

void get_all_paths(string transaction, string defective, unordered_map<string, vector<string> > module_tree, vector<string> nodes) {
    // Mark all the vertices as not visited
    unordered_map<string, bool> visited;
    
    // Create an array to store paths
    string* path = new string[nodes.size()];
    int path_index = 0; // Initialize path[] as empty
    
    // Initialize all vertices as not visited
//    for (int i = 0; i < nodes.size(); i++)
//        visited[i] = false;
    // Call the recursive helper function to print all paths
    print_all_paths_until(transaction, defective, visited, path, path_index, module_tree);
}

void print_all_paths_until(string u, string d, unordered_map<string, bool> visited, string* path, int &path_index, unordered_map<string, vector<string> > module_tree) {
    // Mark the current node and store it in path[]
    visited[u] = true;
    path[path_index] = u;
    path_index++;
    
    // If current vertex is same as destination, then print
    // current path[]
    if (u == d)
    {
        for (int i = 0; i < path_index; i++)
            cout << path[i] << " ";
        cout << endl;
    }
    else // If current vertex is not destination
    {
        // Recur for all the vertices adjacent to current vertex
        vector<string>::iterator i;
        for (i = module_tree[u].begin(); i != module_tree[u].end(); ++i)
            if (!visited[*i])
                print_all_paths_until(*i, d, visited, path, path_index, module_tree);
    }
    
    // Remove current vertex from path[] and mark it as unvisited
    path_index--;
    visited[u] = false;
}

void get_explosion(string transaction, unordered_map<string, vector<string> > module_tree, vector<string> nodes) {
    // Mark all the vertices as not visited
    vector<string> visited;
    stack<unordered_map<string, int> > stack_modules;
    
    unordered_map<string, int> root;
    root[transaction] = 0;
    stack_modules.push(root);
    
    while (!stack_modules.empty()) {
        unordered_map<string, int> module = stack_modules.top();
        stack_modules.pop();
        string vertex;
        int space = 0;
        for (auto& x: module) {
            vertex = x.first;
            space = x.second;
        }
        // Print space and then the module name
        for (int i = 0; i < space; i++) {
            cout << ' ';
        }
        cout << vertex << endl;
        
        if (find(visited.begin(), visited.end(), vertex) == visited.end()) {
            // if vertex not in visited
            visited.push_back(vertex);
            
            unordered_map<string, vector<string> >::const_iterator has_key = module_tree.find(vertex);
            // If vertex is in module_tree
            if (has_key != module_tree.end()) {
                for (auto it = module_tree[vertex].rbegin(); it != module_tree[vertex].rend(); ++it)
                {
                    if (find(visited.begin(), visited.end(), *it) == visited.end()) {
                        // if vertex not in visited
                        unordered_map<string, int> module;
                        module[*it] = space + 1;
                        stack_modules.push(module);
                    } else {
                        // Printing session
                        for (int i = 0; i < space + 1; i++) {
                            cout << ' ';
                        }
                        cout << *it << '*' << endl;
                    }

                }
            }
        }
    }


}


