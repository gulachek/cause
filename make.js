const { CppBuildCommand } = require('gulpachek/cpp');
const fs = require('fs');
const { version } = require('./package.json');
const { Command } = require('commander');

if (!version) {
	console.error(new Error('gtree package.json version not specified'));
	process.exit(1);
}

const program = new Command();
const cppBuild = new CppBuildCommand({
	program,
	cppVersion: 20
});

function makeLib(args) {
	const { cpp } = args;

	const lib = cpp.compile({
		name: 'com.gulachek.error',
		version,
		apiDef: 'GULACHEK_ERROR_API',
		src: [
			'src/error.cpp'
		]
	});

	lib.include('include');

	return lib;
}

cppBuild.build((args) => {
	const { cpp } = args;
	return cpp.toLibrary(makeLib(args)).binary();
});

cppBuild.pack(makeLib);

program.parse();
