###############################################################################
# Copyright 2018 Samsung Electronics All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
###############################################################################

################ AML DataModel build script ##################

import os

Import('env')

caml_env = env.Clone()
target_os = caml_env.get('TARGET_OS')
target_arch = caml_env.get('TARGET_ARCH')
disable_protobuf = caml_env.get('DISABLE_PROTOBUF')

if caml_env.get('RELEASE'):
    caml_env.AppendUnique(CCFLAGS=['-Os'])
else:
    caml_env.AppendUnique(CCFLAGS=['-g'])

caml_env.AppendUnique(CPPPATH=[
        './dependencies/datamodel-aml-cpp/extlibs/pugixml/pugixml-1.8/src',
        './dependencies/datamodel-aml-cpp/include',
        './dependencies/datamodel-aml-cpp/include/logger',
        './include',
        './include/internal'
])

if not disable_protobuf:
    caml_env.AppendUnique(CPPPATH=['./dependencies/datamodel-aml-cpp/protobuf'])
    caml_env.PrependUnique(LIBS=['protobuf'])
else:
    caml_env.AppendUnique(CPPDEFINES = ['_DISABLE_PROTOBUF_'])

if caml_env.get('RELEASE'):
    caml_env.PrependUnique(LIBS=['aml'], LIBPATH=[os.path.join('./dependencies/datamodel-aml-cpp/out/linux/', target_arch, 'release')])
else:
    caml_env.PrependUnique(LIBS=['aml'], LIBPATH=[os.path.join('./dependencies/datamodel-aml-cpp/out/linux/', target_arch, 'debug')])



if target_os not in ['windows']:
    caml_env.AppendUnique(
        CXXFLAGS=['-O2', '-g', '-Wall', '-fPIC', '-fmessage-length=0', '-std=c++0x', '-I/usr/local/include'])

if target_os not in ['windows']:
    caml_env.AppendUnique(LINKFLAGS=['-Wl,--no-undefined'])

if target_os in ['linux']:
    caml_env.AppendUnique(LIBS=['pthread'])

if target_os in ['linux']:
    if not env.get('RELEASE'):
        caml_env.PrependUnique(LIBS=['gcov'])
        caml_env.AppendUnique(CXXFLAGS=['--coverage'])

CAML_DIR = '.'
caml_env.AppendUnique(caml_src = [caml_env.Glob(os.path.join(CAML_DIR, 'src', '*.cpp'))])

amlshared = caml_env.SharedLibrary('caml', caml_env.get('caml_src'))
amlstatic = caml_env.StaticLibrary('caml', caml_env.get('caml_src'))

# Go to build AML DataModel sample apps
if target_os == 'linux':
       SConscript('samples/SConscript')

# Go to build AML DataModel unit test cases
if target_os == 'linux':
    if target_arch in ['x86', 'x86_64']:
        SConscript('unittests/SConscript')