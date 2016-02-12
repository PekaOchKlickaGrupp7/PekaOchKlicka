#pragma once
#include "Button.h"
class ResumeButton :
	public Button
{
public:
	ResumeButton();

	void InitValue() override;
	void ButtonSpecificUpdate() override;
	void ProgressGet() override;
	void ProgressSet() override;

	~ResumeButton();
};

