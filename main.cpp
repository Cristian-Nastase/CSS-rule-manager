#include <iostream>
#include <map>
#include <vector>
#include <cstring>
#include <limits>


// color class


class Color {
private:
    static int noColors;
    static const std::map<char, std::string> colorMap;
    char currentColor;
    bool active;
    int id;

    static void resetColor();
public:
    Color();
    Color(char currentColor, bool active);
    Color(const Color& obj);
    Color& operator=(const Color& obj);
    ~Color();

    char getCurrentColor() const;

    void setCurrentColor(char color);

    friend std::istream& operator>>(std::istream& in, Color& obj);
    friend std::ostream& operator<<(std::ostream& out, const Color& obj);

    void printColored(const char* text);
};

const std::map<char, std::string> Color::colorMap = {
    {'R', "\033[0;31m"},
    {'W', "\033[0;37m"},
    { 'Y', "\033[0;33m"},
    { 'G', "\033[0;32m"},
    { 'B', "\033[0;34m"},
    { 'P', "\033[0;35m"},
    { 'C', "\033[0;36m"}
};

int Color::noColors = 0;

Color::Color() : id(noColors++) {;
    currentColor = 'W';
    active = true;
}

Color::Color(char currentColor, bool active) : id(noColors++) {
    this->currentColor = currentColor;
    this->active = active;
}

Color::Color(const Color& obj) : id(noColors++) {
    this->currentColor = obj.currentColor;
    this->active = obj.active;
}

Color& Color::operator=(const Color& obj) {
    if (this == &obj) {
        return *this;
    }

    this->currentColor = obj.currentColor;
    this->active = obj.active;

    return *this;
}

Color::~Color() {
    noColors--;
}

char Color::getCurrentColor() const {
    return this->currentColor;
}

void Color::setCurrentColor(char color) {
    if (!colorMap.count(color)) {
        std::cout<<"Not a registered color, defaults to white -> "<<color;
        this->currentColor = 'W';
        return;
    }

    this->currentColor = color;
}

std::istream& operator>>(std::istream& in, Color& obj) {
    std::vector<char> options;
    for (auto color: Color::colorMap) {
        options.push_back(color.first);
        std::cout<<options.size()<<". "<<color.first<<"\n";
    }

    std::cout<<"Select a color: ";

    unsigned long option;
    bool valid = false;

    while (!(in >> option) || option == 0 || option > options.size()) {
        std::cout << "Not a possible option, try again\n";
        in.clear();
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    char colorChoosed = options[option - 1];

    obj.setCurrentColor(colorChoosed);

    return in;
}


std::ostream& operator<<(std::ostream& out, const Color& obj) {
    out<<obj.getCurrentColor();
    return out;
}

void Color::resetColor() {
    std::cout<<"\033[0m";
}

void Color::printColored(const char* text) {
    if (!this->active)
        return;

    std::string color = colorMap.at(this->currentColor);

    std::cout<<color<<text;
    resetColor();
}


// box class


class Box {
private:
    static int noBoxes;
    int id;
    int* padding;
    int* margin;
    int border;

    static int* returnVector(int value);

    static void readInt(std::istream& in, int& field);

    void printMargin(int width, int lines) const;
    void printBorder(int width, int m_left, int m_right) const;
    void printPadding(int width, int m_left, int m_right, int lines) const;
    void printContent(char *text, int m_left, int m_right, int p_left, int p_right, Color& color) const;
public:
    Box();
    Box(int* padding, int* margin, int border);
    Box(const Box &obj);
    Box& operator=(const Box &obj);
    ~Box();

    const int* getPadding() const;
    const int* getMargin() const;
    int getBorder() const;

    void setPadding(int* padding);
    void setMargin(int* margin);
    void setBorder(int border);


    friend std::istream& operator>>(std::istream& in, Box& obj);
    friend std::ostream& operator<<(std::ostream& out, const Box& obj);

    void printBox(char* &text, Color& obj);
    int returnTotalWidth(char* text);
    int returnTotalHeight();
};

int Box::noBoxes = 0;

int* Box::returnVector(int value) {
    int* vector = new int[4];

    for (int i = 0; i < 4; i++) {
        vector[i] = value;
    }

    return vector;
}

Box::Box() : id(noBoxes++) {
    padding = returnVector(1);
    margin = returnVector(1);
    border = 1;
}

Box::Box(int* padding, int* margin, int border) : id(noBoxes++) {
    this->padding = returnVector(1);
    for (int i = 0; i < 4; i++) {
        this->padding[i] = padding[i];
    }

    this->margin = returnVector(1);
    for (int i = 0; i < 4; i++) {
        this->margin[i] = margin[i];
    }

    this->border = border;
}

Box::Box(const Box &obj) : id(noBoxes++) {
    this->padding = returnVector(1);
    for (int i = 0; i < 4; i++) {
        this->padding[i] = obj.padding[i];
    }

    this->margin = returnVector(1);
    for (int i = 0; i < 4; i++) {
        this->margin[i] = obj.margin[i];
    }

    this->border = obj.border;
}

Box& Box::operator=(const Box &obj) {
    if (this == &obj) {
        return *this;
    }

    for (int i = 0; i < 4; i++) {
        this->padding[i] = obj.padding[i];
    }

    for (int i = 0; i < 4; i++) {
        this->margin[i] = obj.margin[i];
    }

    this->border = obj.border;

    return *this;
}

Box::~Box() {
    noBoxes--;

    if (padding != nullptr) {
        delete[] padding;
        padding = nullptr;
    }

    if (margin != nullptr) {
        delete[] margin;
        margin = nullptr;
    }
}

const int *Box::getPadding() const {
    return padding;
}

const int* Box::getMargin() const {
    return margin;
}

int Box::getBorder() const {
    return this->border;
}

void Box::setPadding(int *padding) {
    for (int i = 0; i < 4; i++) {
        this->padding[i] = padding[i];
    }
}

void Box::setMargin(int *margin) {
    for (int i = 0; i < 4; i++) {
        this->margin[i] = margin[i];
    }
}

void Box::setBorder(int border) {
    this->border = border;
}

void Box::readInt(std::istream& in, int& field) {

    int temp;
    while (!(in>>temp)) {
        std::cout<<"Enter only natural numbers.\n";
        in.clear();
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    field = temp;
}

std::istream& operator>>(std::istream& in, Box& obj) {
    std::cout<<"Padding (4 values -> top, right, bottom, left): ";
    for (int i = 0; i < 4; i++) {
        Box::readInt(in, obj.padding[i]);
    }

    std::cout<<"Margin (4 values -> top, right, bottom, left): ";
    for (int i = 0; i < 4; i++) {
        Box::readInt(in, obj.margin[i]);
    }

    std::cout<<"Border (1 value): ";
    Box::readInt(in, obj.border);

    return in;
}

std::ostream& operator<<(std::ostream& out, const Box& obj) {
    out<<"Padding: ";
    for (int i = 0; i < 4; i++) {
        out<<obj.padding[i]<<" ";
    }

    out<<"\n";

    out<<"Margin: ";
    for (int i = 0; i < 4; i++) {
        out<<obj.margin[i]<<" ";
    }

    out<<"\n";
    out<<"Border: "<<obj.border;

    return out;
}

int Box::returnTotalWidth(char* text) {
    return padding[1] + margin[1] + padding[3] + margin[3] + strlen(text) + 2 * border;
}

int Box::returnTotalHeight() {
    return padding[0] + padding[2] + margin[0] + margin[2] + 1;
}

void Box::printMargin(int width, int lines) const {
    for (; lines != 0; lines--) {
        for (int i = 0; i < width; i++) std::cout << " ";
    }

    std::cout<<"\n";
}

void Box::printBorder(int width, int m_left, int m_right) const {
    int i, j;

    for (i = border; i != 0; i--) {
        for (j = m_left; j != 0; j--) std::cout<<" ";

        for (j = width - m_left - m_right; j != 0; j--) std::cout << "|";

        for (j = m_right; j != 0; j--) std::cout<<" ";

        std::cout<<"\n";
    }
}

void Box::printPadding(int contentWidth, int m_left, int m_right, int lines) const {
    int i, j;

    for (i = lines; i != 0; i--) {
        for (j = m_left; j != 0; j--) std::cout << " ";

        for (j = border; j != 0; j--) std::cout << "|";

        for (j = contentWidth; j != 0; j--) std::cout << " ";

        for (j = border; j != 0; j--) std::cout << "|";

        for (j = m_right; j != 0; j--) std::cout << " ";

        std::cout<<"\n";
    }
}

void Box::printContent(char *text, int m_left, int m_right, int p_left, int p_right, Color& color) const {
    int j;

    for (j = m_left; j != 0; j--) std::cout << " ";
    for (j = border; j != 0; j--) std::cout << "|";
    for (j = p_left; j != 0; j--) std::cout << " ";

    color.printColored(text);

    for (j = p_right; j != 0; j--) std::cout << " ";
    for (j = border; j != 0; j--) std::cout << "|";
    for (j = m_right; j != 0; j--) std::cout << " ";

    std::cout<<"\n";
}

// margin, padding: 0-top 1-right 2-bottom 3-left
void Box::printBox(char * &text, Color &obj) {
    int i, j;
    int width = this->returnTotalWidth(text);

    int m_top = margin[0];
    int m_right = margin[1];
    int m_bottom = margin[2];
    int m_left = margin[3];

    int p_top = padding[0];
    int p_right = padding[1];
    int p_bottom = padding[2];
    int p_left = padding[3];

    int contentWidth = width - 2 * border - m_left - m_right;

    std::cout<<"\n";

    // on top of content
    printMargin(width, m_top);
    printBorder(width, m_left, m_right);
    printPadding(contentWidth, m_left, m_right, p_top);

    printContent(text, m_left, m_right, p_left, p_right, obj);


    // bottom of content
    printPadding(contentWidth, m_left, m_right, p_bottom);
    printBorder(width, m_left, m_right);
    printMargin(width, m_bottom);

    std::cout<<"\n";
}


// element class


class Element {
private:
    static int noElements;
    char* text;
    char* className;
    char* idName;
    bool isColored;
    Box* boxModel;
    Color* color;
    int id;

    char* returnEmptyString();
    char* testEmptyString(char* text);

    bool stringValid(char* string);
    static void readString(std::istream& in, std::string text, char*& member);
public:
    Element();
    Element(char* text, char* className, char* idName, bool isColored);
    Element(const Element& obj);
    Element& operator=(const Element& obj);
    ~Element();

    const char* getClassName() const;
    const char* getIdName() const;

    void setClassName(char* string);
    void setIdName(char* string);

    friend std::istream& operator>>(std::istream& in, Element& obj);
    friend std::ostream& operator<<(std::ostream& out, const Element& obj);

    void printElement();
};

int Element::noElements = 0;

char* Element::returnEmptyString() {
    char* p = new char[4];
    strcpy(p,"N/A");

    return p;
}

Element::Element() : boxModel(new Box), color(new Color), id(noElements++) {
    text = returnEmptyString();
    className = returnEmptyString();
    idName = returnEmptyString();

    isColored = true;
}

char* Element::testEmptyString(char* text) {
    if (text == nullptr) {
        return returnEmptyString();
    }

    char* copy = new char[strlen(text) + 1];
    strcpy(copy, text);
    return copy;
}

Element::Element(char* text, char* className, char* idName, bool isColored) : boxModel(new Box), color(new Color), id(noElements++) {
    this->text = testEmptyString(text);
    this->className = testEmptyString(className);
    this->idName = testEmptyString(idName);

    this->isColored = isColored;
}

Element::Element(const Element& obj) : boxModel(new Box(*obj.boxModel)), color(new Color(*obj.color)), id(noElements++) {
    this->text = new char[strlen(obj.text) + 1];
    strcpy(this->text, obj.text);

    this->className = new char[strlen(obj.className) + 1];
    strcpy(this->className, obj.className);

    this->idName = new char[strlen(obj.idName) + 1];
    strcpy(this->idName, obj.idName);

    this->isColored = obj.isColored;
}

Element& Element::operator=(const Element& obj) {
    if (this == &obj) {
        return *this;
    }

    delete[] this->className;
    this->className = new char[strlen(obj.className) + 1];
    strcpy(this->className, obj.className);

    delete[] this->idName;
    this->idName = new char[strlen(obj.idName) + 1];
    strcpy(this->idName, obj.idName);

    delete[] this->text;
    this->text = new char[strlen(obj.text) + 1];
    strcpy(this->text, obj.text);

    this->isColored = obj.isColored;

    delete boxModel;
    this->boxModel = new Box(*obj.boxModel);

    delete color;
    this->color = new Color(*obj.color);

    return *this;
}

Element::~Element() {
    noElements--;
    delete boxModel;
    delete color;
    delete[] className;
    delete[] idName;
    delete[] text;
}

const char *Element::getClassName() const {
    return this->className;
}

const char *Element::getIdName() const {
    return this->idName;
}

bool Element::stringValid(char *string) {
    char blockedCharacters[100];
    strcpy(blockedCharacters, "~!@$%^&*()+=,./';:?><[]{}|`#\\ \"");

    for (int i = 0 ; i < strlen(string); i++) {
        if (strchr(blockedCharacters, string[i]) != nullptr)
            return false;
    }

    return true;
}

void Element::setClassName(char *string) {
    delete[] this->className;

    if (!stringValid(string)) {
        std::cout<<"Class name invalid, will set to N/A.";

        this->className = new char[4];
        strcpy(className, "N/A");

        return;
    }

    this->className = new char[strlen(string) + 1];
    strcpy(this->className, string);
}

void Element::setIdName(char *string) {
    delete[] this->idName;

    if (!stringValid(string)) {
        std::cout<<"Id name invalid, will set to N/A.";

        this->idName = new char[4];
        strcpy(idName, "N/A");

        return;
    }

    this->idName = new char[strlen(string) + 1];
    strcpy(this->idName, string);
}

void Element::readString(std::istream& in, std::string textToPrint, char*& member) {
    char *buffer = new char[256];
    std::cout<<textToPrint;
    in.getline(buffer, 256);

    delete[] member;
    member = new char[strlen(buffer) + 1];
    strcpy(member, buffer);

    delete[] buffer;
}

std::istream& operator>>(std::istream& in, Element& obj) {
    in.ignore(1000, '\n');
    Element::readString(in,"Enter a text: ", obj.text);
    Element::readString(in, "Enter a class name: ", obj.className);
    Element::readString(in, "Enter an id: ", obj.idName);

    in>>*obj.boxModel;
    in>>*obj.color;

    return in;
}

std::ostream& operator<<(std::ostream &out, const Element &obj) {
    out<<"Text: "<<obj.text<<"\n";
    out<<"Class Name: "<<obj.className<<"\n";
    out<<"Id:"<<obj.idName<<"\n";

    out<<"Box model: "<<*obj.boxModel<<"\n";
    out<<"Color: "<<*obj.color;

    return out;
}

void Element::printElement() {
    this->boxModel->printBox(this->text, *this->color);
}

// selector class


class Selector {
    private:
        static int noSelector;
        char* selectorString;
        char type;
        std::vector<Element> elements;
        bool isEmpty;
        int id;

        static bool validSelector(char* string);

        bool checkClassName(const Element* element) const;
        bool checkIdName(const Element* element) const;

    public:
        Selector();
        Selector(char* selectorString);
        Selector(const Selector &obj);
        Selector& operator=(const Selector &obj);
        ~Selector();

        std::string getSelectorString() const;
        void setSelectorString(char* string);

        friend std::istream& operator>>(std::istream& in, Selector& obj);
        friend std::ostream& operator<<(std::ostream& out, const Selector& obj);

        void createElement();
        void addElement(Element& element);

        void verifyType();


        std::vector<const Element*> returnTargetedElements();
};

int Selector::noSelector = 0;

Selector::Selector() : id(noSelector++) {
    selectorString = new char[4];
    strcpy(selectorString, "N/A");

    type = '*';
    isEmpty = true;
}

Selector::Selector(char* selectorString) : id(noSelector++) {
    this->selectorString = new char[strlen(selectorString) + 1];
    strcpy(this->selectorString, selectorString);

    isEmpty = true;

    verifyType();
}

Selector::Selector(const Selector &obj) : id(noSelector++) {
    this->selectorString = new char[strlen(obj.selectorString) + 1];
    strcpy(this->selectorString, obj.selectorString);

    this->type = obj.type;

    if (obj.isEmpty) {
        this->isEmpty = obj.isEmpty;
        return;
    }

    for (int i = 0; i < obj.elements.size(); i++) {
        this->elements.push_back(obj.elements[i]);
    }
}

Selector& Selector::operator=(const Selector &obj) {
    if (this == &obj)
        return *this;

    delete[] this->selectorString;
    this->selectorString = new char[strlen(obj.selectorString) + 1];
    strcpy(this->selectorString, obj.selectorString);

    while (!elements.empty()) {
        this->elements.pop_back();
    }

    this->isEmpty = obj.isEmpty;
    for (int i = 0; i < obj.elements.size(); i++) {
        this->elements.push_back(obj.elements[i]);
    }

    this->type = obj.type;

    return *this;
}

Selector::~Selector() {
    noSelector--;
    delete[] selectorString;
}

std::string Selector::getSelectorString() const {
    return this->selectorString;
}

bool Selector::validSelector(char *string) {
    char blockedCharacters[100];
    strcpy(blockedCharacters, "~!@$%^&*()+=,/';:?><[]{}|`\\ \"");

    for (int i = 0 ; i < strlen(string); i++) {
        if (strchr(blockedCharacters, string[i]) != nullptr)
            return false;
    }

    return true;
}

void Selector::verifyType() {
    char lastChr = '-';
    for (int i = 0; i < strlen(this->selectorString); i++) {
        if (selectorString[i] == '.' || selectorString[i] == '#') {
            if (lastChr != '-') {
                std::cout<<"Compound selector, not allowed here, defaults to '*'.\n";
                this->type = '*';
                return;
            }

            lastChr = selectorString[i];
        }
    }

    if (lastChr == '-') {
        std::cout<<"Element selector, not allowed here, defaults to '*'. \n";
        this->type = '*';
        return;
    }

    if (lastChr == '.') {
        this->type = 'C';
    }

    if (lastChr == '#') {
        this->type = 'I';
    }

    char* copy = new char[strlen(this->selectorString)];
    strcpy(copy, this->selectorString + 1);

    delete[] this->selectorString;
    selectorString = new char[strlen(copy) + 1];
    strcpy(selectorString, copy);

    delete[] copy;
}

void Selector::setSelectorString(char *string) {
    delete[] selectorString;

    if (!validSelector(string)) {
        std::cout<<"Invalid selector entered, will target all elements as default.";
        this->type = '*';

        this->selectorString = new char[4];
        strcpy(this->selectorString, "N/A");
        return;
    }

    this->selectorString = new char[strlen(string) + 1];
    strcpy(this->selectorString, string);

    verifyType();
}


std::istream& operator>>(std::istream &in, Selector &obj) {
    std::cout<<"Type what elements you want to select: ";

    char buffer[256];
    in>>buffer;

    obj.setSelectorString(buffer);

    return in;
}

std::ostream& operator<<(std::ostream &out, const Selector &obj) {
    out<<"\nSelector: "<<obj.getSelectorString()<<"\nElements it affects: ";

    for (int i = 0; i < obj.elements.size(); i++) {
        out<<"Element"<<i<<": \n"<<obj.elements[i]<<"\n";
    }

    return out;
}

void Selector::createElement() {
    std::cout<<"\nElement "<<this->elements.size() + 1<<": \n\n";
    Element e;
    std::cin>>e;

    this->elements.push_back(e);
    isEmpty = false;
}

void Selector::addElement(Element& element) {
    Element copy = element;
    this->elements.push_back(copy);
}

bool Selector::checkClassName(const Element *element) const {
    return !strcmp(element->getClassName(), this->selectorString);
}

bool Selector::checkIdName(const Element *element) const {
    return !strcmp(element->getIdName(), this->selectorString);
}

std::vector<const Element*> Selector::returnTargetedElements() {
    std::vector<const Element*> targetedElements;

    if (this->type != '*') {
        for (int i = 0; i < this->elements.size(); i++) {
            if (this->type == 'C' && checkClassName(&elements[i])) {
                targetedElements.push_back(&elements[i]);
            }

            else if (this->type == 'I' && checkIdName(&elements[i])) {
                targetedElements.push_back(&elements[i]);
            }
        }

        return targetedElements;
    }

    for (int i = 0; i < this->elements.size(); i++) {
        targetedElements.push_back(&this->elements[i]);
    }

    return targetedElements;
}

int main() {
    return 0;
}