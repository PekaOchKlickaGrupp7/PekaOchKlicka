#pragma once
class Node
{
public:
	Node();
	Node(bool aIsBlocked);
	~Node();

	void SetParent(Node* aNode);
	void SetG(const float aValue);
	void SetH(const float aValue);

	void SetX(const int aValue);
	void SetY(const int aValue);

	int GetX() const;
	int GetY() const;

	Node* GetParent();
	bool GetIsBlocked() const
	{
		return myIsBlocked;
	}

	float GetG() const;
	float GetH() const;
	float GetF() const
	{
		return myG + myH;
	}
private:
	float myG;
	float myH;
	int myX;
	int myY;
	Node* myParent;
	bool myIsBlocked;
};
