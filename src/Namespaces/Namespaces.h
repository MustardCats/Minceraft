#pragma once

#ifndef MISC
#define MISC

namespace misc {
	struct floatcoord {
		float x, y;
	};
	struct intcoord {
		int x, y;
	};
	struct tcoord {
		tcoord(int sx, int sy, int sz) {
			x = sx;
			y = sy;
			z = sz;
		}
		bool operator!= (tcoord& other) {
			if (x != other.x || y != other.y || z != other.z)
				return false;
			else
				return true;
		}
		tcoord() {}

		int x;
		int y;
		int z;
	};
	struct uvcoords {
		uvcoords() {
			ul.x = ul.y = ur.x = ur.y = dl.x = dl.y = dr.x = dr.y = 0;
		}
		floatcoord ul, ur, dl, dr;
	};
	struct int_tcoord {
		int x, y, z;
	};
	struct CanMove {
		CanMove() {
			nx = px = ny = py = nz = pz = true;
		}
		bool nx, px, ny, py, nz, pz;
	};
	struct chunkindex {
		int c, bx, by, bz;
	};

	static const tcoord chunkSize(16, 16, 16);
};

#endif