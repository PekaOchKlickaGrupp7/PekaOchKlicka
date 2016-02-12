#pragma once
#include "Button.h"
class FullscreenButton :
	public Button
{
public:
	FullscreenButton();

	void InitValue() override;
	void ButtonSpecificUpdate() override;
	void ProgressGet() override;
	void ProgressSet() override;

	~FullscreenButton();
};

