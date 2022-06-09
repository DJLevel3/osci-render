package sh.ball.parser.lua;

import org.luaj.vm2.LuaValue;
import sh.ball.audio.FrameSource;
import sh.ball.shapes.Vector2;

import javax.script.Bindings;
import javax.script.CompiledScript;
import javax.script.SimpleBindings;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

public class LuaSampleSource implements FrameSource<Vector2> {

  private Bindings bindings = new SimpleBindings();
  private CompiledScript lastWorkingScript;
  private String lastWorkingTextScript;
  private CompiledScript script;
  private String textScript;
  private boolean active = true;
  private long step = 1;
  private Set<String> programDefinedVariables = new HashSet<>();

  public void setScript(String textScript, CompiledScript script) {
    if (lastWorkingScript == null) {
      lastWorkingScript = script;
      lastWorkingTextScript = textScript;
    }
    this.script = script;
    this.textScript = textScript;
  }

  @Override
  public Vector2 next() {
    try {
      boolean updatedFile = false;

      bindings.put("step", (double) step);

      LuaValue result;
      try {
        result = (LuaValue) script.eval(bindings);
        if (!lastWorkingTextScript.equals(textScript)) {
          lastWorkingScript = script;
          lastWorkingTextScript = textScript;
          updatedFile = true;
        }
      } catch (Exception e) {
        result = (LuaValue) lastWorkingScript.eval(bindings);
      }
      step++;

      if (updatedFile) {
        // reset variables
        step = 1;
        List<String> variablesToRemove = new ArrayList<>();
        bindings.keySet().forEach(name -> {
          if (!programDefinedVariables.contains(name)) {
            variablesToRemove.add(name);
          }
        });
        variablesToRemove.forEach(name -> bindings.remove(name));
      }

      return new Vector2(result.get(1).checkdouble(), result.get(2).checkdouble());
    } catch (Exception ignored) {}

    return new Vector2();
  }

  @Override
  public boolean isActive() {
    return active;
  }

  @Override
  public void disable() {
    active = false;
  }

  @Override
  public void enable() {
    active = true;
  }

  @Override
  public void setFrameSettings(Object settings) {}

  @Override
  public Object getFrameSettings() {
    return null;
  }

  public void setVariable(String variableName, Object value) {
    programDefinedVariables.add(variableName);
    bindings.put(variableName, value);
  }

  public void resetStep() {
    step = 1;
  }
}