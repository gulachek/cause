const { CppBuildCommand } = require('gulpachek/cpp');
const fs = require('fs');
const { version } = require('./package.json');
const { Command } = require('commander');
const { series } = require('bach');
const { spawn } = require('child_process');

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

const test = program.command('test')
.description('Build and run unit tests');

cppBuild.configure(test, (args) => {
	const { cpp, sys } = args;

	const lib = makeLib(args);

	const test = cpp.compile({
		name: 'error_test',
		src: ['test/error_test.cpp']
	});

	const ut = cpp.require('org.boost.unit-test-framework', '1.78.0', 'dynamic');

	test.link(lib);
	test.link(ut);

	const exe = test.executable();

	return series(sys.rule(exe), () =>
		spawn(exe.abs(), [], { stdio: 'inherit' }));
});

cppBuild.pack(makeLib);

program.parse();
