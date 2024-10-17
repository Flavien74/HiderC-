#pragma once
class TransformUI
{
public:
    TransformUI();
    TransformUI(int posX, int posY, int width, int height, float ancX, float ancY);

    int getPositionX() const;
    int getPositionY() const;
    float getAnchorX() const;
    float getAnchorY() const;
    int getWidth() const;
    int getHeight() const;

    void setPosition(int x, int y);
    void setAnchor(float x, float y);
    void setSize(int width, int height);

    int newGetPositionX() const;
    int newGetPositionY() const;

private:
    float positionX, positionY;

    float width, height;

    float anchorX, anchorY;
};

