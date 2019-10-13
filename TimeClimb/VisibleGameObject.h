#pragma once


class VisibleGameObject
{
public:
	VisibleGameObject();
	virtual ~VisibleGameObject();

	virtual void Load(std::string filename);
	virtual void Draw(sf::RenderWindow & window);
	virtual void Update(sf::Event& event);
	virtual bool getClickState();
	virtual void Restart();
	virtual void setUnbreakable(bool ans);
	virtual bool getUnbreakable();
	virtual void setFinished(bool res);
	virtual bool getFinished();
	virtual void reInit();
	
	virtual void SetPosition(float x, float y);
	virtual sf::Vector2f GetPosition() const;
	virtual bool IsLoaded() const;

	virtual float GetWidth() const;
	virtual float GetHeight() const;
	virtual void setClickState(bool ans);
	virtual sf::Vector2f getCenter();

	void setPlased(bool res);
	bool getPlased();

	virtual sf::Rect<float> GetBoundingRect() const;

protected:
	sf::Sprite& GetSprite();

private:
	sf::Sprite  _sprite;
	sf::Texture _texture;
	sf::Image _image;
	std::string _filename;
	bool _isLoaded;
	bool plased;
	bool finished;
};