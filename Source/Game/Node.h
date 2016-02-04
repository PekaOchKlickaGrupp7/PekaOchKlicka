#pragma once
class Node
{
public:
	Node();
	Node(int aX, int aY, bool aIsBlocked);
	~Node();

	void SetParent(Node* aNode);
	void SetG(const int aValue);
	void SetH(const int aValue);

	void SetX(const int aValue);
	void SetY(const int aValue);

	int GetX() const;
	int GetY() const;

	Node* GetParent();
	bool GetIsBlocked() const
	{
		return myIsBlocked;
	}

	void SetPath(bool aIsPath);
	bool GetPath() { return myIsPath; }

	int GetG() const;
	int GetH() const;
	int GetF() const
	{
		return myG + myH;
	}
private:
	int myG;
	int myH;
	int myX;
	int myY;
	bool myIsPath;
	Node* myParent;
	bool myIsBlocked;
};
