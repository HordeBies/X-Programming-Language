#ifndef ANALYZER
#define ANALYZER

#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <stack>
#include <unordered_map>

#include "../parse_tree/parse_tree.h"

#include "../utils/utilities.h"
#include "../utils/constants.h"

typedef std::unordered_map<std::string, DeclarationNode*> SymbolTable;

struct Scope {
    ScopeType type;         // The type of the scope
    Node* ptr;              // A pointer to the node of the scope
    SymbolTable table;      // The symbol table of this scope

    Scope(ScopeType type, Node* ptr = NULL) {
        this->type = type;
        this->ptr = ptr;
    }
};

class Analyzer
{
private:
    std::string sourceFilename;
    std::vector<std::string> sourceCode;
    std::vector<Scope*> scopes;
    std::unordered_map<std::string, int> aliases;
    std::vector<std::pair<int, DeclarationNode*>> symbols;    // Used just for printing the symbol table. NOT IMPORTANT!

    void readSourceCode() {
        std::ifstream fin(sourceFilename);

        if (!fin.is_open()) {
            return;
        }

        std::string line;
        while (getline(fin, line)) {
            sourceCode.push_back(Utils::replaceTabsWithSpaces(line));
        }

        fin.close();
    }

public:
    bool declareFuncParams = false;
    bool initializeVar = false;
        
    Analyzer(const std::string& sourceFilename) {
        this->sourceFilename = sourceFilename;
        this->readSourceCode();
    }

    bool isGlobalScope() {
        return scopes.size() == 1;
    }

    FunctionNode* getFunctionScope() {
        for (int i = (int) scopes.size() - 1; i >= 0; --i) {
            if (scopes[i]->type == SCOPE_FUNCTION) {
                return (FunctionNode*) scopes[i]->ptr;
            }
        }

        return NULL;
    }

    void addScope(ScopeType type, Node* ptr = NULL) {
        scopes.push_back(new Scope(type, ptr));
    }

    void popScope() {
        Scope* scope = scopes.back();
        scopes.pop_back();

        for (auto& it : scope->table) {
            DeclarationNode* sym = it.second;

            if (sym->used <= 0) {
                if (dynamic_cast<VarDeclarationNode*>(sym)) {
                    log("the value of variable '" + sym->declaredHeader() + "' is never used", sym->id->loc, LOG_WARNING);
                }
                else if (sym->id->name != "main") {
                    log("function '" + sym->declaredHeader() + "' is never called", sym->id->loc, LOG_WARNING);
                }
            }
            
            aliases[sym->id->name]--;
        }

        delete scope;
    }

    bool declareSymbol(DeclarationNode* sym) {
        SymbolTable& table = scopes.back()->table;

        if (table.count(sym->id->name)) {
            return false;
        }

        symbols.push_back({ scopes.size() - 1, sym });

        int num = aliases[sym->id->name]++;

        if (num > 0) {
            sym->alias = sym->id->name + "_" + std::to_string(num);
        } else {
            sym->alias = sym->id->name;
        }

        table[sym->id->name] = sym;
        return true;
    }

    DeclarationNode* getSymbol(const std::string& identifier) {
        for (int i = (int) scopes.size() - 1; i >= 0; --i) {
            if (scopes[i]->table.count(identifier)) {
                return scopes[i]->table[identifier];
            }
        }

        return NULL;
    }

    bool hasBreakScope() {
        for (int i = (int) scopes.size() - 1; i >= 0; --i) {
            if (scopes[i]->type == SCOPE_LOOP) {
                return true;
            }
        }

        return false;
    }

    bool hasFunctionScope() {
        for (int i = (int) scopes.size() - 1; i >= 0; --i) {
            if (scopes[i]->type == SCOPE_FUNCTION) {
                return true;
            }
        }

        return false;
    }

    std::string getSymbolTableStr() {
        std::stringstream ss;

        ss << "scope,type,identifier,alias,used\n";

        for (int i = 0; i < symbols.size(); ++i) {
            int scope = symbols[i].first;
            DeclarationNode* sym = symbols[i].second;

            ss << scope;
            ss << "," << sym->declaredType();
            ss << "," << sym->id->name;
            ss << "," << sym->alias;
            ss << "," << sym->used << "\n";
        }

        return ss.str();
    }

    void log(const std::string& msg, const Location& loc, LogLevel level) {
        std::string logLvl;

        switch (level) {
            // In future we may change the output stream of each level
            // and change th colors of the log level and the intended token
            case LOG_ERROR:
                logLvl = "error";
                break;
            case LOG_WARNING:
                logLvl = "warning";
                break;
            case LOG_NOTE:
                logLvl = "note";
                break;
        }

        fprintf(stdout, "%s:%d:%d: %s: %s\n", sourceFilename.c_str(), loc.lineNum, loc.pos, logLvl.c_str(), msg.c_str());
        fprintf(stdout, "%s\n", sourceCode[loc.lineNum - 1].c_str());
        fprintf(stdout, "%*s", loc.pos, "^");

        if (loc.len > 1) {
            fprintf(stdout, "%s", std::string(loc.len - 1, '~').c_str());
        }

        fprintf(stdout, "\n");
    }

};

#endif