#pragma once
#include <memory>
#include <string>

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
			std::unique_ptr<EFModel> m_model;
			std::unique_ptr<EFView> m_view;

			bool run();
			void loadSimulation(std::string& url);
	
		private:

	};
	
#endif