#include "TransformUI.h"
#include <iostream>

TransformUI::TransformUI()
    : positionX(0), positionY(0), width(100), height(100), anchorX(0.5f), anchorY(0.5f) {}

TransformUI::TransformUI(int posX, int posY, int w, int h, float ancX, float ancY)
    : positionX(posX), positionY(posY), width(w), height(h), anchorX(ancX), anchorY(ancY) {}

int TransformUI::getPositionX() const { return positionX; }
int TransformUI::getPositionY() const { return positionY; }
float TransformUI::getAnchorX() const { return anchorX; }
float TransformUI::getAnchorY() const { return anchorY; }
int TransformUI::getWidth() const { return width; }
int TransformUI::getHeight() const { return height; }

void TransformUI::setPosition(int x, int y) {
    positionX = x;
    positionY = y;
}

void TransformUI::setAnchor(float x, float y) {
    anchorX = x;
    anchorY = y;
}

void TransformUI::setSize(int w, int h) {
    width = w;
    height = h;
}

int TransformUI::newGetPositionX() const {
    return positionX - (anchorX * width);
}

int TransformUI::newGetPositionY() const {
    return positionY - (anchorY * height);
}
