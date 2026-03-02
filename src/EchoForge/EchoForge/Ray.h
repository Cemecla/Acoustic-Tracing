#pragma once

#include <vector>
#include <chrono>

#ifndef __Ray__
	#define __Ray__
	
	class Ray
	{
		private:
			std::vector<float> m_startPosition;
			std::vector<float> m_lastSeen;
			float m_energy;
			std::chrono::high_resolution_clock::time_point m_time ;
			std::vector<float> m_velocity;
			std::vector<float> m_currentPosition;

	public:
		//
		// Méthode pour retirer l'énergie lors d'une collision
		// - Prends un float de 0 à 1 pour l'absorption du matériel
		void loseEnergy(float obsorption);

		float getEnergy();
		
		// Méthode pour optenir la position actuelle du rayon
		std::vector<float> getPosition();

		//Méthode pour obtenir le temps depuis l'envoye du rayon
		int getTimeDelta();

		// Constructeur d'un Rayon
		Ray(std::vector<float> position, std::vector<float> velocity);


	};
	

#endif // !__Ray__


