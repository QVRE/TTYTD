project('ttytd', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

ttytd_sources = [ 'src/main.c', 'src/Elafri.c', 'src/graphics.c']
cc = meson.get_compiler('c')
m_dep = cc.find_library('m', required : false)

executable('ttytd',
           ttytd_sources,
           install : true,
           dependencies : m_dep)
