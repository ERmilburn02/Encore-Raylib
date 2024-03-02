//look this is the first header file ive ever created


int notesHit = 0;
int notesMissed = 0;
int perfectHit = 0;

int combo = 0;
int score = 0;

bool overdrive = false;

bool FC = true;

float health = 100.0f;

int multiplier(int instrument) {
		int od = overdrive ? 2 : 1;
	
	if (instrument == 1 || instrument == 3){ 

		if (combo < 11) { return 1 * od; }
		else if (combo < 21) { return 2 * od; }
		else if (combo < 31) { return 3 * od; }
		else if (combo < 41) { return 4 * od; }
		else if (combo < 51) { return 5 * od; }
		else if (combo > 50) { return 6 * od; }
		else { return 1 * od; };
	}
	else {
		if (combo < 11) { return 1 * od; }
		else if (combo < 21) { return 2 * od; }
		else if (combo < 31) { return 3 * od; }
		else if (combo > 30) { return 4 * od; }
		else { return 1 * od; }
	};
}




// clone hero defaults
float starThreshold[7] = { 0.3f, 0.7f, 1.0f, 2.0f, 2.8f, 3.6f, 4.4f };



class player {
public:
	static void resetPlayerStats() {
		notesHit = 0;
		notesMissed = 0;

		combo = 0;
		score = 0;
		FC = true;
	};

	static void HitNote(bool perfect, int instrument) {
		notesHit += 1;
		combo += 1;
		float perfectMult = perfect ? 1.2f : 1.0f;
		score += (int)((30 * (multiplier(instrument)) * perfectMult));
		perfectHit += perfect ? 1 : 0;
	}
	static void MissNote() {
		notesMissed += 1;
		combo = 0;
		FC = false;
	}
};

