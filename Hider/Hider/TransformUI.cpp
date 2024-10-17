#include "TransformUI.h"
#include <iostream>

TransformUI::TransformUI()
    : positionX(0), positionY(0), width(100), height(100), anchorX(0.5f), anchorY(0.5f) {}

TransformUI::TransformUI(float posX, float posY, float w, float h, float ancX, float ancY)
    : positionX(posX), positionY(posY), width(w), height(h), anchorX(ancX), anchorY(ancY) {}

float TransformUI::getPositionX() const { return positionX; }
float TransformUI::getPositionY() const { return positionY; }
float TransformUI::getAnchorX() const { return anchorX; }
float TransformUI::getAnchorY() const { return anchorY; }
float TransformUI::getWidth() const { return width; }
float TransformUI::getHeight() const { return height; }

void TransformUI::setPosition(float x, float y) {
    positionX = x;
    positionY = y;
}

void TransformUI::setAnchor(float x, float y) {
    anchorX = x;
    anchorY = y;
}

void TransformUI::setSize(float w, float h) {
    width = w;
    height = h;
}

float TransformUI::newGetPositionX() const {
    return positionX - (anchorX * width);
}

float TransformUI::newGetPositionY() const {
    return positionY - (anchorY * height);
}
