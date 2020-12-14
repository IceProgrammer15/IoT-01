const { src,series } = require('gulp');
const run = require('gulp-run');
const clean = require('gulp-clean');

const destFolder = '../esp/switch/data';

const yarnBuild = ()=>{
    return run(`yarn build --dest ${destFolder}`).exec();
}

const cleanup = ()=>{
    return src([`${destFolder}/**/*.map`,`${destFolder}/preact_prerender_data.json`], {read: false})
    .pipe(clean({force:true}));
}

const build = series(yarnBuild, cleanup);

exports.yarnBuild = yarnBuild;
exports.default = build;