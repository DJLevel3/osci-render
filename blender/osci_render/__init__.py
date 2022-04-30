bl_info = {
    "name": "osci-render",
    "author": "James Ball", 
    "version": (1, 0, 2),
    "blender": (3, 1, 2),
    "location": "View3D",
    "description": "Addon to send gpencil frames over to osci-render",
    "warning": "Requires a camera and gpencil object",
    "wiki_url": "https://github.com/jameshball/osci-render",
    "category": "Development",
}

import bpy
import bmesh
import socket
import json
import atexit
from bpy.app.handlers import persistent

HOST = "localhost"
PORT = 51677

sock = None


class OBJECT_PT_osci_render_settings(bpy.types.Panel):
    bl_idname = "OBJECT_PT_osci_render_settings"
    bl_label = "osci-render settings"
    bl_space_type = "PROPERTIES"
    bl_region_type = "WINDOW"
    bl_context = "render"

    def draw_header(self, context):
        layout = self.layout

    def draw(self, context):
        global sock
        if sock is None:
            self.layout.operator("render.osci_render_connect", text="Connect to osci-render")
        else:
            self.layout.operator("render.osci_render_close", text="Close osci-render connection")
            

class osci_render_connect(bpy.types.Operator):
    bl_label = "Connect to osci-render"
    bl_idname = "render.osci_render_connect"
    bl_description = "Connect to osci-render"

    def execute(self, context):
        global sock
        if sock is None:
            try:
                sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                sock.settimeout(1)
                sock.connect((HOST, PORT))
                send_scene_to_osci_render(bpy.context.scene)
            except socket.error as exp:
                sock = None
                self.report({"WARNING"}, "Failed to connect to osci-render - make sure it is running first!")
                return {"CANCELLED"}
                
        return {"FINISHED"}


class osci_render_close(bpy.types.Operator):
    bl_label = "Close osci-render connection"
    bl_idname = "render.osci_render_close"

    def execute(self, context):
        close_osci_render()
        
        return {"FINISHED"}


@persistent
def close_osci_render():
    global sock
    if sock is not None:
        try:
            sock.send("CLOSE\n".encode('utf-8'))
            sock.close()
        except socket.error as exp:
            sock = None


def append_matrix(object_info, obj):
    camera_space = bpy.context.scene.camera.matrix_world.inverted() @ obj.matrix_world
    object_info["matrix"] = [camera_space[i][j] for i in range(4) for j in range(4)]
    return object_info


@persistent
def send_scene_to_osci_render(scene):
    global sock
    engine_info = {"objects": []}

    if sock is not None:
        for obj in bpy.data.objects:
            if obj.visible_get() and obj.type == 'GPENCIL':
                object_info = {"name": obj.name}
                strokes = obj.data.layers.active.frames.data.active_frame.strokes                    
                
                object_info["vertices"] = []
                for stroke in strokes:
                    object_info["vertices"].append([{
                        "x": vert.co[0],
                        "y": vert.co[1],
                        "z": vert.co[2],
                    } for vert in stroke.points])
                
                engine_info["objects"].append(append_matrix(object_info, obj))


        engine_info["focalLength"] = -0.05 * bpy.data.cameras[0].lens

        json_str = json.dumps(engine_info, separators=(',', ':')) + '\n'
        try:
            sock.sendall(json_str.encode('utf-8'))
        except socket.error as exp:
            sock = None


operations = [OBJECT_PT_osci_render_settings, osci_render_connect, osci_render_close]


def register():
    bpy.app.handlers.frame_change_pre.append(send_scene_to_osci_render)
    bpy.app.handlers.depsgraph_update_post.append(send_scene_to_osci_render)
    atexit.register(close_osci_render)
    for operation in operations:
        bpy.utils.register_class(operation)


def unregister():
    bpy.app.handlers.frame_change_pre.remove(send_scene_to_osci_render)
    bpy.app.handlers.depsgraph_update_post.remove(send_scene_to_osci_render)
    atexit.unregister(close_osci_render)
    for operation in reversed(operations):
        bpy.utils.unregister_class(operation)


if __name__ == "__main__":
    register()
