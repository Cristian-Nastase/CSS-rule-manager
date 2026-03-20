#include <iostream>
#include <map>
#include <vector>
#include <cstring>

// color class

class Color {
private:
    // static const std::map<char, char*> colorMap;
    char currentColor;
    bool active;
public:
    Color();
    Color(char currentColor, bool active);
    Color(const Color& obj);
    Color& operator=(const Color& obj);
    ~Color();

    friend std::istream& operator>>(std::istream& in, Color& obj);
    friend std::ostream& operator<<(std::ostream& out, const Color& obj);

    char getCurrentColor() const;
    void setCurrentColor(char color);
    void resetColor();

    void printColored(const char* text);

};

void Color::resetColor() {
    std::cout<<"\033[0m";
}

void Color::printColored(const char* text) {
    if (!this->active)
        return;

    char key = this->getCurrentColor();
    // const char* color = colorMap.at(key);

    // std::cout<<color<<text;
    this->resetColor();
}

char Color::getCurrentColor() const {
    return this->currentColor;
}

void Color::setCurrentColor(char color) {
    this->currentColor = color;
}

Color::Color() {;
    currentColor = 'W';
    active = true;
}

Color::Color(char currentColor, bool active) {
    this->currentColor = currentColor;
    this->active = active;
}

Color::Color(const Color& obj) {
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
    return;
}

std::istream& operator>>(std::istream& in, Color& obj) {
    std::cout<<"Select a color: ";

    int option;

    in>>option;

    if (option == 1) {
        obj.setCurrentColor('R');
    }

    return in;
}


std::ostream& operator<<(std::ostream& out, const Color& obj) {
    out<<obj.getCurrentColor();
    return out;
}

// box class

class Box {
private:
    static int noBoxes;
    int id;
    int* padding;
    int* margin;
    int border;

    int* returnVector(int value);
    void printMargin(int width, int lines) const;
    void printBorder(int width, int m_left, int m_right) const;
    void printPadding(int width, int m_left, int m_right, int lines) const;
    void printContent(char *text, int m_left, int m_right, int p_left, int p_right) const;
public:
    Box();
    Box(int* padding, int* margin, int border);
    Box(const Box &obj);
    Box& operator=(const Box &obj);
    ~Box();

    friend std::istream& operator>>(std::istream& in, Box& obj);
    friend std::ostream& operator<<(std::ostream& out, const Box& obj);

    void printBox(char* &text, const Color& obj);
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

int Box::returnTotalWidth(char* text) {
    return padding[1] + margin[1] + padding[3] + margin[3] + strlen(text) + 2 * border;
}

int Box::returnTotalHeight() {
    return padding[0] + padding[2] + margin[0] + margin[2] + 1;
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

std::istream& operator>>(std::istream& in, Box& obj) {
    std::cout<<"Padding (4 values -> top, right, bottom, left): ";
    for (int i = 0; i < 4; i++) {
        in>>obj.padding[i];
    }

    std::cout<<"Margin (4 values -> top, right, bottom, left): ";
    for (int i = 0; i < 4; i++) {
        in>>obj.margin[i];
    }

    std::cout<<"Border (1 value): ";
    in>>obj.border;

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

void Box::printMargin(int width, int lines) const {
    for (; lines != 0; lines--) {
        for (int i = 0; i < width; i++) std::cout << " ";
    }

    std::cout<<"\n";
}

void Box::printBorder(int contentWidth, int m_left, int m_right) const {
    int i, j;

    for (i = border; i != 0; i--) {
        for (j = m_left; j != 0; j--) std::cout<<" ";

        for (j = border; j != 0; j--) std::cout << "|";

        for (j = contentWidth; j != 0; j--) std::cout << "-";

        for (j = border; j != 0; j--) std::cout << "|";

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

void Box::printContent(char *text, int m_left, int m_right, int p_left, int p_right) const {
    int j;

    for (j = m_left; j != 0; j--) std::cout << " ";
    for (j = border; j != 0; j--) std::cout << "|";
    for (j = p_left; j != 0; j--) std::cout << " ";

    std::cout << text;

    for (j = p_right; j != 0; j--) std::cout << " ";
    for (j = border; j != 0; j--) std::cout << "|";
    for (j = m_right; j != 0; j--) std::cout << " ";

    std::cout<<"\n";
}

// margin, padding: 0-top 1-right 2-bottom 3-left
void Box::printBox(char * &text, const Color &obj) {
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
    printBorder(contentWidth, m_left, m_right);
    printPadding(contentWidth, m_left, m_right, p_top);

    printContent(text, m_left, m_right, p_left, p_right);


    // bottom of content
    printPadding(contentWidth, m_left, m_right, p_bottom);
    printBorder(contentWidth, m_left, m_right);
    printMargin(width, m_bottom);

    std::cout<<"\n";
}


// element class

class Element {
private:
    static int noElements;
    char* text;
    char* className;
    char* id;
    bool isColored;
    Box* boxModel;
    Color* color;

    char* returnEmptyString();
    char* testEmptyString(char* text);
    void readString(std::istream& in, std::string text, char*& member);
public:
    Element();
    Element(char* text, char* className, char* id, bool isColored);
    Element(const Element& obj);
    Element& operator=(const Element& obj);
    ~Element();

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

char* Element::testEmptyString(char* text) {
    if (text == nullptr) {
        return returnEmptyString();
    }

    char* copy = new char[strlen(text) + 1];
    strcpy(copy, text);
    return copy;
}

Element::Element() : boxModel(new Box), color(new Color) {
    noElements++;

    text = returnEmptyString();
    className = returnEmptyString();
    id = returnEmptyString();

    isColored = true;
}

Element::Element(char* text, char* className, char* id, bool isColored) : boxModel(new Box), color(new Color) {
    noElements++;

    this->text = testEmptyString(text);
    this->className = testEmptyString(className);
    this->id = testEmptyString(id);

    this->isColored = isColored;
}

Element::Element(const Element& obj) : boxModel(new Box(*obj.boxModel)), color(new Color(*obj.color)) {
    noElements++;

    this->text = new char[strlen(obj.text) + 1];
    strcpy(this->text, obj.text);

    this->className = new char[strlen(obj.className) + 1];
    strcpy(this->className, obj.className);

    this->id = new char[strlen(obj.id) + 1];
    strcpy(this->id, obj.id);

    this->isColored = obj.isColored;
}

Element& Element::operator=(const Element& obj) {
    if (this == &obj) {
        return *this;
    }

    delete[] this->className;
    this->className = new char[strlen(obj.className) + 1];
    strcpy(this->className, obj.className);

    delete[] this->id;
    this->id = new char[strlen(obj.id) + 1];
    strcpy(this->id, obj.id);

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
    delete[] id;
    delete[] text;
}

void Element::readString(std::istream& in, std::string textToPrint, char*& member) {
    char *buffer = new char[256];
    std::cout<<textToPrint;
    in>>buffer;

    delete[] member;
    member = new char[strlen(buffer) + 1];
    strcpy(member, buffer);

    delete[] buffer;
}

std::istream& operator>>(std::istream& in, Element& obj) {
    obj.readString(in,"Enter a text: ", obj.text);
    obj.readString(in, "Enter a class name: ", obj.className);
    obj.readString(in, "Enter an id: ", obj.id);

    in>>*obj.boxModel;
    in>>*obj.color;

    return in;
}

std::ostream &operator<<(std::ostream &out, const Element &obj) {
    out<<"Text: "<<obj.text<<"\n";
    out<<"Class Name: "<<obj.className<<"\n";
    out<<"Id:"<<obj.id<<"\n";

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
        char* selectorString;
        int type;
        std::vector<Element> element;
        bool isEmpty;
    public:
        Selector();
        Selector(char* selectorString, bool cascades);
        Selector(const Selector &obj);
        Selector& operator=(const Selector &obj);
        ~Selector();
};

int main() {
    Element e;
    e.printElement();
    return 0;
}