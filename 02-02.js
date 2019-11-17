const spin = require('./spinners');
const fs = require('fs');
const lines = [
	'#include "02-02.h"',
	'',
];

Object.prototype.forEach = function(callback) {
	Object.keys(this).forEach((k)=>{
		callback.call(this, this[k], k);
	});
};

spin.forEach(function(v, k) {
	lines.push('static const char * const frames_' + k + '[]={"' + v.frames.join('","').replace(/\\/g, '\\\\') + '",NULL};');
});

lines.push('');
lines.push('spinner_t spinners[] = {');

var n = 0;
spin.forEach(function(v, k) {
	n++;
	lines.push('	{.name="' + k + '",.interval=' + (v.interval * 1000) + ',.frames=frames_' + k + ',.iframe=0},');
});
lines[lines.length-1] = lines[lines.length-1].substr(0, lines[lines.length-1].length-1);
lines.push('};');
lines.push('unsigned int nspinner = ' + n + ';');

fs.writeFileSync('02-02.c', lines.join('\n'));
