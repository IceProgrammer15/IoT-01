const { src,series,dest } = require('gulp');
const run = require('gulp-run');
const clean = require('gulp-clean');
const gzip = require('gulp-gzip');


const destFolder = '../esp/switch/data';

const yarnBuild = ()=>{
    return run(`yarn build --dest ${destFolder}`).exec();
}

const cleanup = ()=>{
    return src([`${destFolder}/**/*.map`,`${destFolder}/preact_prerender_data.json`,`${destFolder}/**/*.{js,css,html}`], {read: false})
    .pipe(clean({force:true}));
}

const compress = ()=>{
    return src([`${destFolder}/**/*.{js,css,html}`])
    .pipe(gzip())
    .pipe(dest(`${destFolder}`));
}

const build = series(yarnBuild,  compress, cleanup);

exports.yarnBuild = yarnBuild;
exports.default = build;