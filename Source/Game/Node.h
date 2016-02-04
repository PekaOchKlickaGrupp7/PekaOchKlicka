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
	Node* myParent;
	bool myIsBlocked;
};
