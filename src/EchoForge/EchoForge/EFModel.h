#pragma once

#include "json.hpp"
#include <string>

using json = nlohmann::json;

class EFController;
class Map;

#ifndef EF_MODEL_H
	#define EF_MODEL_H

	class EFModel
	{
		public:
			/**
			*	Main constructor
			*	desc: The EFModel is where the logic reside and take the Controller as the parent
			*   param parent: EFController
			*/
			EFModel(EFController* parent);

			/**
			*	desc:
			*   param name: string
			*	return: json
			*/
			json loadJson(std::string name);

			/**
			*	desc:
			*	param data: json
			*   param name: string
			*/
			bool saveJson(json data, std::string name);
	
			
		
		private:
			EFController* m_parent;
	};

#endif
