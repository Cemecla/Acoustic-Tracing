#pragma once
#include <Memory>

class EFModel;
class EFView;

#ifndef __EFController__
	#define __EFController__
	class EFController
	{
		public:
			/**
			*	Default constructor
			*/
			EFController();
	
		private:
			std::unique_ptr<EFModel> m_model;
			std::unique_ptr<EFView> m_view;
	
			bool mainLoop();
	};

#endif