#pragma once
class TransformUI
{
public:
    TransformUI();
    TransformUI(float posX, float posY, float width, float height, float ancX, float ancY);

    float getPositionX() const;
    float getPositionY() const;
    float getAnchorX() const;
    float getAnchorY() const;
    float getWidth() const;
    float getHeight() const;

    void setPosition(float x, float y);
    void setAnchor(float x, float y);
    void setSize(float width, float height);

    float newGetPositionX() const;
    float newGetPositionY() const;

private:
    float positionX, positionY;

    float width, height;

    float anchorX, anchorY;
};

