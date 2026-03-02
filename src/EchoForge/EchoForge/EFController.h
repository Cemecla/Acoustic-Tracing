#pragma once
#include <Memory>

class EFModel;
class EFView;

class EFController
{
	public:
		EFController();

	private:
		std::unique_ptr<EFModel> m_model;
		std::unique_ptr<EFView> m_view;
};