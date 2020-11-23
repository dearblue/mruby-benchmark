# mruby-benchmark


## できること


## くみこみかた

`build_config.rb` に gem として追加して、mruby をビルドして下さい。

```ruby
MRuby::Build.new do |conf|
  conf.gem "mruby-benchmark", github: "dearblue/mruby-benchmark"
end
```

- - - -

mruby gem パッケージとして依存したい場合、`mrbgem.rake` に記述して下さい。

```ruby
# mrbgem.rake
MRuby::Gem::Specification.new("mruby-XXX") do |spec|
  ...
  spec.add_dependency "mruby-benchmark", github: "dearblue/mruby-benchmark"
end
```


## つかいかた


## Specification

  - Package name: mruby-benchmark
  - Version: 0.1
  - Product quality: CONCEPT
  - Author: [dearblue](https://github.com/dearblue)
  - Project page: <https://github.com/dearblue/mruby-benchmark>
  - Licensing: [2 clause BSD License](LICENSE)
  - Dependency external mrbgems: (NONE)
  - Bundled C libraries (git-submodules): (NONE)
