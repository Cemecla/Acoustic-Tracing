#pragma once

class EFModel;
class EFView;

class EFController
{
	public:
		EFController();
		~EFController();

	private:
		EFModel* model_;
		EFView* view_;
};