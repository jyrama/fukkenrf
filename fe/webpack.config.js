var path = require('path');
var webpack = require('webpack');

const ExtractCssChunks = require('extract-css-chunks-webpack-plugin');
// const BundleAnalyzerPlugin = require('webpack-bundle-analyzer').BundleAnalyzerPlugin;

module.exports = {
  // devtool: 'eval',
  // mode: 'development',
  mode: 'production',
  entry: [
    // 'webpack-dev-server/client?http://localhost:3000',
    './src/index'
  ],
  output: {
    path: path.join(__dirname, 'dist'),
    filename: 'bundle.js',
    publicPath: '/static/'
  },
  resolve: {
    extensions: ['.js', '.ts', '.tsx'],
    alias: { mobx: __dirname + "/node_modules/mobx/lib/mobx.es6.js" }
  },
  plugins: [
    new ExtractCssChunks({
      filename: '[name].css',
      chunkFilename: '[id].css',
    })
  ],
  module: {
    rules: [{
      test: /\.tsx?$/,
      loader: "ts-loader",
      include: path.join(__dirname, 'src')
    },
    {
      test: /\.less$/,
        use: [
          {
            loader: ExtractCssChunks.loader
          },
          {
            loader: 'css-loader'
          },
          {
            loader: 'less-loader',
            options: {  
                javascriptEnabled: true
            },
          },
        ],
    },
    {
      test: /\.css$/i,
      use: [ExtractCssChunks.loader, 'css-loader'],
    },
    ]
  }
};
