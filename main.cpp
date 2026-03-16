#include <cstring>
#include <iostream>

class Selector {
    private:
        static int blocksNumber;
        int* specificity;
        char* selectorString;
        bool cascades;
        bool compoundSelector;
        int* returnNewSpecificity();
        int* calculateSpecificity(const char *selectorString);
        bool checkCompound(const char *selectorString);
    public:
        Selector();
        Selector(char* selectorString, bool cascades);
        Selector(const Selector &obj);
        Selector& operator=(const Selector &obj);
        ~Selector();
};

// 0. id 1. class 2. element
int* returnNewSpecificity() {
    int* specificity = new int[3];
    for (int i = 0; i < 3; i++) {
        specificity[i] = 0;
    }

    return specificity;
}

bool checkCompound(const char *selectorString) {
    char *combinators = strcpy(new char[7], ":> ~+|");

    for (int i = 0; i < strlen(combinators); i++) {
        if (strchr(selectorString, combinators[i]) != nullptr) {
            delete[] combinators;
            combinators = nullptr;
            return true;
        }
    }

    delete[] combinators;
    combinators = nullptr;

    return false;
}

int* calculateSpecificity(const char *selectorString) {
    int* specificity = returnNewSpecificity();

    bool lastCharSpace;
    for (int i = 0; i < strlen(selectorString); i++) {
        switch (selectorString[i]) {
            case '#':
                specificity[0]++;
                break;
            case '.':
                specificity[1]++;
                break;
            case ' ':
                lastCharSpace = true;
                break;
            default:
                if (!lastCharSpace)
                    break;

                specificity[2]++;
                lastCharSpace = false;
        }
    }

    return specificity;
}

Selector::Selector() {
    blocksNumber++;
    selectorString = strcpy(new char[2], "*");
    cascades = true;
    compoundSelector = false;
    specificity = returnNewSpecificity();
}

Selector::Selector(char* selectorString, bool cascades) {
    blocksNumber++;
    this->selectorString = strcpy(new char[strlen(selectorString) + 1], selectorString);
    this-> cascades = cascades;
    specificity = calculateSpecificity(selectorString);
    compoundSelector = checkCompound(selectorString);
}

Selector::Selector(const Selector &obj) {
    blocksNumber++;
    selectorString = strcpy(new char[strlen(obj.selectorString) + 1], obj.selectorString);
    cascades = obj.cascades;
    compoundSelector = obj.compoundSelector;
    specificity = new int[3];
    for (int i = 0; i < 3; i++) {
        specificity[i] = obj.specificity[i];
    }
}

Selector& Selector::operator=(const Selector &obj) {
    if (this == &obj)
        return *this;

    delete[] selectorString;
    selectorString = strcpy(new char[strlen(obj.selectorString) + 1], selectorString);

    cascades = obj.cascades;
    compoundSelector = obj.compoundSelector;

    delete[] specificity;
    specificity = new int[3];
    for (int i = 0; i < 3; i++) {
        specificity[i] = obj.specificity[i];
    }

    return *this;
}

Selector::~Selector() {
    delete[] specificity;
    delete[] selectorString;
}

class Property {
    private:
        static double totalNumber;
        const int line;
        char* name;
        bool affectsRendering;
        int valuesAcceptedNumber;
    public:
        Property();
        Property(char* name, bool affectsRendering);
        Property(char* name, bool affectsRendering, int valuesAcceptedNumber);
        Property(const Property &property);
        Property& operator=(const Property &property);
        ~Property();
};

class Value {
    private:
        float* numericValues;
        char valueType;
        char* measureOfUnit;
        bool multipleValues;
    public:
    Value();
    Value(float* numericValues, char valueType);
    Value(float* numericValues, char valueType, char* measureOfUnit);
    Value(float* numericValues, char valueType, char* measureOfUnit, bool multipleValues);
    Value(const Value& value);
    Value& operator=(const Value& value);
    ~Value();
};

class Function {
    private:
        static int totalNumber;
        const int id;
        const bool inLine;
        char* name;
        int argumentsAccepted;
        char* returnType;
    public:
        Function();
        Function(char* name);
        Function(char* name, int argumentsAccepted, char* returnType);
        Function(char* name, bool inLine, int argumentsAccepted, char* returnType);
        Function(const Function &function);
        Function& operator=(const Function &function);
        ~Function();
};

int main() {
}