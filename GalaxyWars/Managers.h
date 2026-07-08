#pragma once
#ifndef MANAGERS_H
#define MANAGERS_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include "Ships.h"
#include "GameException.h"

using namespace std;

namespace GalaxyWars {

    class LevelManager {
        int currentlevel;
        int level1score;
        int level2score;
    public:
        LevelManager() : currentlevel(1), level1score(500), level2score(1500) {}

        LevelManager(const LevelManager& other) : currentlevel(other.currentlevel), level1score(other.level1score), level2score(other.level2score) {
            cout << "LevelManager copied at level: " << currentlevel << endl;
        }

        void updateLevel(int score) {
            if (score >= level2score)
                currentlevel = 3;
            else if (score >= level1score)
                currentlevel = 2;
            else
                currentlevel = 1;
        }

        int getLevel() const {
            return currentlevel;
        }

        float getEnemySpeed() const {
            if (currentlevel == 1)
                return 2.0f;
            if (currentlevel == 2)
                return 3.0f;
            return 3.5f;
        }

        float getSpawnInterval() const {
            if (currentlevel == 1)
                return 1.0f;
            if (currentlevel == 2)
                return 0.6f;
            return 0.3f;
        }
    };

    class ScoreManager {
        struct ScoreEntry {
            string name;
            int score;
            float time;
        };

        static void sortScores(ScoreEntry scores[], int count)
        {
            // bubble sort descending
            for (int idx = 0; idx < count - 1; idx++) {
                for (int jdx = 0; jdx < count - idx - 1; jdx++) {
                    if (scores[jdx].score < scores[jdx + 1].score) {
                        ScoreEntry temp = scores[jdx];
                        scores[jdx] = scores[jdx + 1];
                        scores[jdx + 1] = temp;
                    }
                }
            }
        }

    public:
        static void saveScore(const string& name, int score, float timetaken)
        {
            const int MAXSCORES = 50;
            ScoreEntry scores[MAXSCORES];
            int count = 0;

            ifstream infile("scores.txt");
            if (infile.is_open())
            {
                while (count < MAXSCORES && infile >> scores[count].name >> scores[count].score >> scores[count].time) {
                    count++;
                }
                infile.close();
            }

            if (count < MAXSCORES) {
                scores[count].name = name;
                scores[count].score = score;
                scores[count].time = timetaken;
                count++;
            }
            else {
                scores[MAXSCORES - 1].name = name;
                scores[MAXSCORES - 1].score = score;
                scores[MAXSCORES - 1].time = timetaken;
            }

            sortScores(scores, count);

            if (count > MAXSCORES)
                count = MAXSCORES;

            ofstream outfile("scores.txt");
            if (!outfile.is_open()) {
                throw GameException("Could not save scores");
            }
            for (int idx = 0; idx < count; idx++) {
                outfile << scores[idx].name << " " << scores[idx].score << " " << scores[idx].time << endl;
            }
            outfile.close();
        }

        static void saveBinaryScore(const string& name, int score, float time)
        {
            ofstream file("scores.dat", ios::binary | ios::app);
            if (!file.is_open()) {
                throw GameException("Cannot open binary score file");
            }

            int namelen = name.length();
            file.write(reinterpret_cast<const char*>(&namelen), sizeof(int));
            file.write(name.c_str(), namelen);
            file.write(reinterpret_cast<const char*>(&score), sizeof(int));
            file.write(reinterpret_cast<const char*>(&time), sizeof(float));
            file.close();
        }

        static void loadBinaryScores()
        {
            ifstream file("scores.dat", ios::binary);
            if (!file.is_open()) {
                cout << "No binary scores found" << endl;
                return;
            }

            cout << endl << "--- Binary Scores ---" << endl;
            while (file.peek() != EOF)
            {
                int namelen;
                file.read(reinterpret_cast<char*>(&namelen), sizeof(int));

                char* namebuf = new char[namelen + 1];
                file.read(namebuf, namelen);
                namebuf[namelen] = '\0';
                string name(namebuf);
                delete[] namebuf;

                int score;
                float time;
                file.read(reinterpret_cast<char*>(&score), sizeof(int));
                file.read(reinterpret_cast<char*>(&time), sizeof(float));

                cout << name << " - " << score << " (" << time << "s)" << endl;
            }
            file.close();
        }

        static void displayHighScoresOnImage(sf::RenderWindow& window, sf::Font& font)
        {
            ifstream file("scores.txt");

            string name;
            int score;
            float timetaken;
            sf::Text text;
            text.setFont(font);
            text.setCharacterSize(24);
            text.setFillColor(sf::Color::Yellow);
            text.setStyle(sf::Text::Bold);

            float ypos = 200;
            int count = 0;

            while (file >> name >> score >> timetaken && count < 10)
            {
                text.setString(to_string(count + 1) + ". " + name + " - " + to_string(score));
                text.setPosition(280, ypos);
                window.draw(text);
                ypos += 35;
                count++;
            }
            file.close();
        }
    };

}

#endif
