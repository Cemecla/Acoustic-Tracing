#pragma once
#include <memory>

class EFModel;
class EFView;

#ifndef EF_CONTROLLER_H
	#define EF_CONTROLLER_H
	class EFController {
		public:
			/**
			*	Default constructor
			*/
			EFController();

			bool run();
	
		private:
			std::unique_ptr<EFModel> m_model;
			std::unique_ptr<EFView> m_view;

	};
	
#endif