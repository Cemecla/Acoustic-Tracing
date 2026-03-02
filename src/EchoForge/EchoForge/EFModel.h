#pragma once

class EFController;

class EFModel
{
	public:
		EFModel(EFController* parent);
		~EFModel();
	
	private:
		EFController* m_parent;
};
